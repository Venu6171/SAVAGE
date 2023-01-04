// Description: Post Processing shader.

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

float range = 0.0f;

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position, 1.0f);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);

	color += textureMap.Sample(textureSampler, input.texCoord) * 2.0f;
	color += textureMap.Sample(textureSampler, input.texCoord + float2(-range, 0));
	color += textureMap.Sample(textureSampler, input.texCoord + float2(range, 0));

	color /= 4;

	return color;
}