// Description: Shader for recording pixel depths.

cbuffer TransformBuffer : register(b0)
{
	matrix wvp;
}

cbuffer SettingsBuffer : register(b3)
{
	int useBumpMap;
	float depthDistance;
}

cbuffer BoneTransformBuffer : register(b4)
{
	matrix boneTransforms[256];
}

// If you want to use displacement map (bump map), you need
// to add a SettingsBuffer here so you can control the
// displacement scale.

Texture2D displacementMap : register(t0);
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
	float4 position2 : TEXCOORD;
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
	if(useBumpMap == 1)
	{
		displacement = displacementMap.SampleLevel(textureSampler, input.texCoord, 0).x;
	}

	float3 position = input.position + (input.normal * displacement * 0.2f);

	matrix toLightNDC = wvp;

	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
	toLightNDC = mul(boneTransform, wvp);

	VS_OUTPUT output;
	output.position = mul(float4(position, 1.0f), toLightNDC);
	output.position2 = mul(float4(position, 1.0f), toLightNDC);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float depth = depthDistance - input.position2.z / input.position2.w;
	return float4(depth, 1.0f, 1.0f, 1.0f);
}