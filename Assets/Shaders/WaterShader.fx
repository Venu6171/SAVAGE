// Reference: https://www.rastertek.com/dx10tut29.html

cbuffer ConstantBuffer : register(b0)
{
	matrix world;
	matrix projectionMatrix;
	matrix reflectionMatrix;
	float3 viewPosition;
}

cbuffer SettingsBuffer : register(b1)
{
	float waterTranslation;
	float reflectRefractScale;
}

Texture2D reflectionTexture : register(t0);
Texture2D refractionTexture : register(t1);
Texture2D normalTexture : register(t2);
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
	float4 reflectionPosition: TEXCOORD1;
	float4 refractionPosition: TEXCOORD2;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	matrix reflectProjectWorld;
	matrix viewProjectWorld;

	// Change the position vector to be 4 units for proper matrix calculations.
	input.position.w = 1.0f;

	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.position = mul(input.position, world);
	//output.position = mul(output.position, float4(viewPosition, 1.0f));
	output.position = mul(output.position, projectionMatrix);

	// Store the texture coordinates for the pixel shader.
	output.texCoord = input.texCoord;
	//Create the reflection projection world matrix just like the reflection tutorial and calculate the reflection coordinates from it.

	// Create the reflection projection world matrix.
	reflectProjectWorld = mul(reflectionMatrix, projectionMatrix);
	reflectProjectWorld = mul(world, reflectProjectWorld);

	// Calculate the input position against the reflectProjectWorld matrix.
	output.reflectionPosition = mul(input.position, reflectProjectWorld);
	//Refraction coordinates are calculated in the same way as the reflection coordinates except that we use a view projection world matrix for them.

	// Create the view projection world matrix for refraction.
	viewProjectWorld = mul(reflectProjectWorld, projectionMatrix);
	viewProjectWorld = mul(world, viewProjectWorld);

	// Calculate the input position against the viewProjectWorld matrix.
	output.refractionPosition = mul(input.position, viewProjectWorld);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float2 reflectTexCoord;
	float2 refractTexCoord;
	float4 normalMap;
	float3 normal;
	float4 reflectionColor;
	float4 refractionColor;
	float4 color;
	//Just like the translate shader tutorial we use a translation variable updated each frame to move the water normal map texture along the Y axis to simulate motion.

	// Move the position the water normal is sampled from to simulate moving water.	
	input.texCoord.y += waterTranslation;
	//Convert both the reflection and refraction coordinates into texture coordinates in the - 1 to + 1 range.

	// Calculate the projected reflection texture coordinates.
	reflectTexCoord.x = input.reflectionPosition.x / input.reflectionPosition.w / 2.0f + 0.5f;
	reflectTexCoord.y = -input.reflectionPosition.y / input.reflectionPosition.w / 2.0f + 0.5f;

	// Calculate the projected refraction texture coordinates.
	refractTexCoord.x = input.refractionPosition.x / input.refractionPosition.w / 2.0f + 0.5f;
	refractTexCoord.y = -input.refractionPosition.y / input.refractionPosition.w / 2.0f + 0.5f;
	//Sample the normal for this pixel from the normal map and expand the range to be in the - 1 to + 1 range.

	// Sample the normal from the normal map texture.
	normalMap = normalTexture.Sample(textureSampler, input.texCoord);

	// Expand the range of the normal from (0,1) to (-1,+1).
	normal = (normalMap.xyz * 2.0f) - 1.0f;
	//Now distort the reflection and refraction coordinates by the normal map value.This creates the rippling effect by using the normal transitioning from - 1 to + 1 to distort our view just as water waves distort light.The normal map value is multiplied by the reflectRefractScale to make the ripples less pronounced and more natural looking.

	// Re-position the texture coordinate sampling position by the normal map value to simulate the rippling wave effect.
	reflectTexCoord = reflectTexCoord + (normal.xy * reflectRefractScale);
	refractTexCoord = refractTexCoord + (normal.xy * reflectRefractScale);
	//Now sample the reflection and refraction pixel based on the updated texture sampling coordinates.

	// Sample the texture pixels from the textures using the updated texture coordinates.
	reflectionColor = reflectionTexture.Sample(textureSampler, reflectTexCoord);
	refractionColor = refractionTexture.Sample(textureSampler, refractTexCoord);
	//Finally combine the reflection and refraction pixel using a linear interpolation.

	// Combine the reflection and refraction results for the final color.
	color = lerp(reflectionColor, refractionColor, 0.6f);

	return color;
}