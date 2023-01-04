cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix wvp;
	float3 viewPosition;
}

cbuffer SettingsBuffer : register(b1)
{
	float fogStart;
	float fogEnd;
	float padding[2];
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);


struct VS_INPUT
{
	float4 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD0;
	float fogFactor : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	float4 cameraPosition;

	input.position.w = 1.0f;

	output.position = mul(input.position, wvp);

	output.texCoord = input.texCoord;

	matrix view = mul(world, wvp);

	cameraPosition = mul(input.position, world);
	cameraPosition = mul(input.position, view);

	output.fogFactor = saturate(fogEnd - cameraPosition.z) / (fogEnd - fogStart);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float4 textureColor;
	float4 fogColor;
	float4 finalColor;


	// Sample the texture pixel at this location.
	textureColor = textureMap.Sample(textureSampler, input.texCoord);

	// Set the color of the fog to grey.
	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f);
	//The fog color equation then does a linear interpolation between the texture color and the fog color based on the fog factor.

	// Calculate the final color using the fog effect equation.
	finalColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;

	return finalColor;
}
