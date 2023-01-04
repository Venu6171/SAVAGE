cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

cbuffer BoneTransformBuffer : register(b4)
{
	matrix boneTransforms[256];
}

Texture2D textureMap : register(t0);
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
	float2 texCoord : TEXCOORD;
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
	matrix toLightNDC = wvp;
	matrix boneTransform = GetBoneTransform(input.blendIndices, input.blendWeights);
	toLightNDC = mul(boneTransform, wvp);

	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.0f), toLightNDC);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	return textureMap.Sample(textureSampler, input.texCoord);
}