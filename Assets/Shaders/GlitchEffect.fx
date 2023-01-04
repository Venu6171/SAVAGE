cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

cbuffer SettingsBuffer : register(b1)
{
	float intensity;
	float colorIntensity;
	float flipUp;
	float flipDown;
}

Texture2D textureMap : register(t0);
Texture2D displayMap : register(t1);
SamplerState textureSampler : register(s0);

float displace;
float scale;
float filterRadius;
float4 direction = {1.0f, 1.0f, 1.0f, 1.0f};

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
	output.position = mul(float4(input.position, 1.0f), wvp);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 normal = textureMap.Sample(textureSampler, input.texCoord.xy * scale);

	input.texCoord.y -= (1 - (input.texCoord.y + flipUp)) * step(input.texCoord.y, flipUp) + (1 - (input.texCoord.y - flipDown)) * step(flipDown, input.texCoord.y);

	input.texCoord.xy += (normal.xy - 0.5) * displace * intensity;

	float4 color = textureMap.Sample(textureSampler, input.texCoord.xy);
	float4 redcolor = textureMap.Sample(textureSampler, input.texCoord.xy + direction.xy * 0.01 * filterRadius * colorIntensity);
	float4 greencolor = textureMap.Sample(textureSampler, input.texCoord.xy - direction.xy * 0.01 * filterRadius * colorIntensity);

	color += float4(redcolor.r, redcolor.b, redcolor.g, 1) * step(filterRadius, -0.001);
	color *= 1 - 0.5 * step(filterRadius, -0.001);

	color += float4(greencolor.g, greencolor.b, greencolor.r, 1) * step(0.001, filterRadius);
	color *= 1 - 0.5 * step(0.001, filterRadius);

	return color;
}
