// Reference: https://www.rastertek.com/dx10tut32.html

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

cbuffer SettingsBuffer : register(b1)
{
	float refractionScale;
}

Texture2D textureMap : register(t0);
Texture2D normalTexture : register(t1);
Texture2D refractionMap : register(t2);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD0;
    float4 refractionPosition : TEXCOORD1;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix refraction;

	output.position = mul(float4(input.position, 1.0f), wvp);
	output.texCoord = input.texCoord;

    output.refractionPosition = mul(float4(input.position, 1.0f), wvp);
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float2 refractTexCoord;
    float4 normalMap;
    float3 normal;
    float4 refractionColor;
    float4 textureColor;
    float4 color;

	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
	refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;

	// Sample the normal from the normal map texture.
	// Get normal from texture and convert from [0,1] to [-1, 1].
	normalMap = normalTexture.Sample(textureSampler, input.texCoord);
	normal = normalize((normalMap.xyz * 2.0f) - 1.0f);
	
	// Re-position the texture coordinate sampling position by the normal map value to simulate light distortion through glass.
	refractTexCoord = refractTexCoord + (normal.xy * refractionScale);
	
	// Sample the texture pixel from the refraction texture using the perturbed texture coordinates.
	refractionColor = refractionMap.Sample(textureSampler, refractTexCoord);
	
	// Sample the texture pixel from the glass color texture.
	textureColor = textureMap.Sample(textureSampler, input.texCoord);
	
	// Evenly combine the glass color and refraction value for the final color.
	color = lerp(refractionColor, textureColor, 0.5f);
	
	return color;
}
