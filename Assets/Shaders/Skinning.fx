cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp[2];
	float3 viewPosition;
}

cbuffer LightBuffer : register(b1)
{
	float3 lightDirection;
	float4 lightAmbient;
	float4 lightDiffuse;
	float4 lightSpecular;
}

cbuffer MaterialBuffer : register(b2)
{
	float4 materialAmbient;
	float4 materialDiffuse;
	float4 materialSpecular;
	float specularPower;
}

cbuffer SettingsBuffer : register(b3)
{
	int useSpecularMap;
	int useBumpMap;
	int useNormalMap;
	int useShadow;
	int useSkinning;
	float depthBias;
}

cbuffer BoneTransformBuffer : register(b4)
{
	matrix boneTransforms[256];
}

Texture2D diffuseMap : register(t0);
Texture2D specularMap : register(t1);
Texture2D displacementMap : register(t2);
Texture2D normalMap : register(t3);
Texture2D shadowMap : register(t4);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
	int4 blendIndices : BLENDINDICES;
	float4 blendWeights: BLENDWEIGHT;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float3 normal : TEXCOORD0;
	float3 tangent : TEXCOORD1;
	float3 binormal : TEXCOORD2;
	float3 dirToView : TEXCOORD3;
	float2 texCoord : TEXCOORD4;
	float4 position2 : TEXCOORD5;
};

static matrix Identity =
{
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
};

matrix GetBoneTransform(int4 indices, float4 weights)
{
	if (length(weights) <= 0.0f)
		return Identity;

	matrix transform;
	transform = boneTransforms[indices[0]] * weights[0];
	transform += boneTransforms[indices[1]] * weights[1];
	transform += boneTransforms[indices[2]] * weights[2];
	transform += boneTransforms[indices[3]] * weights[3];
	return transform;
}

VS_OUTPUT VS(VS_INPUT input)
{
	float displacement = 0.0f;

	if (useBumpMap == 1)
	{
		displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	}

	float3 position = input.position + (input.normal * displacement * 0.2f);

	matrix toWorld = world;
	matrix toNDC = wvp[0];
	matrix toLightNDC = wvp[1];

	if (useSkinning == 1)
	{
		matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
		toWorld = mul(boneTransform, world);
		toNDC = mul(boneTransform, wvp[0]);
		toLightNDC = mul(boneTransform, wvp[1]);
	}

	float3 worldPosition = mul(float4(position, 1.0f), toWorld).xyz;
	float3 worldNormal = mul(input.normal, (float3x3)toWorld);
	float3 worldTangent = mul(input.tangent, (float3x3)toWorld);
	float3 worldBinormal = cross(worldNormal, worldTangent);

	VS_OUTPUT output;
	output.position = mul(float4(position, 1.0f), toNDC);
	output.normal = worldNormal;
	output.tangent = worldTangent;
	output.binormal = worldBinormal;

	output.dirToView = normalize(viewPosition - worldPosition);
	output.texCoord = input.texCoord;
	output.position2 = mul(float4(position, 1.0f), toLightNDC);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 dirToView = normalize(input.dirToView);
	float3 normal = normalize(input.normal);

	if (useNormalMap == 1)
	{
		// True normal to use for lighting
		// Get normal from texture and convert from [0,1] to [-1, 1]
		float3 sampledNormal = normalMap.Sample(textureSampler, input.texCoord).xyz;
		float3 unpackedNormal = normalize((sampledNormal * 2.0f) - 1.0f);

		// Fix normals from rasterizer and construct the tangent space matrix
		float3 n = normal;
		float3 b = normalize(input.binormal);
		float3 t = normalize(input.tangent);

		float3x3 tbnw = float3x3(t, b, n);
		normal = mul(unpackedNormal, tbnw);
	}

	float4 ambient = lightAmbient * materialAmbient;

	float3 dirToLight = -lightDirection;
	float diffuseIntensity = saturate(dot(normal, dirToLight));
	float4 diffuse = diffuseIntensity * lightDiffuse * materialDiffuse;

	float3 reflected = normalize(reflect(lightDirection, normal));
	float specularBase = saturate(dot(reflected, dirToView));
	float specularIntensity = pow(specularBase, specularPower);
	float4 specular = specularIntensity * lightSpecular * materialSpecular;

	float4 diffuseMapColor = diffuseMap.Sample(textureSampler, input.texCoord);

	float4 specularMapColor = 1.0f;

	if (useSpecularMap == 1)
	{
		specularMapColor = specularMap.Sample(textureSampler, input.texCoord);
	}

	float realDepth = 1.0f - input.position2.z / input.position2.w;
	float2 shadowUV = input.position2.xy / input.position2.w;
	shadowUV = (shadowUV + 1.0f) * 0.5f;
	shadowUV.y = 1.0f - shadowUV.y;

	float recordedDepth = shadowMap.Sample(textureSampler, shadowUV).r;

	float4 finalColor;

	if (realDepth + depthBias < recordedDepth)
	{
		finalColor = ambient * diffuseMapColor;
	}
	else
		finalColor = (ambient + diffuse) * diffuseMapColor + (specular * specularMapColor.r);

	return finalColor;
}