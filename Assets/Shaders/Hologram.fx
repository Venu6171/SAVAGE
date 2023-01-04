// Description: Simple shader that does transform and lighting.
// Reference: https://sharpcoderblog.com/blog/create-a-hologram-effect-in-unity-3d

cbuffer TransformBuffer : register(b0)
{
	matrix world;
	matrix wvp;
	float3 viewPosition;
}

cbuffer SettingsBuffer : register(b1)
{
	float scale;
	float scrollSpeed;
	float glowIntensity;
	float glitchSpeed;
}

Texture2D textureMap : register(t0);
Texture2D alphaTextureMap : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float2 texCoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_Position;
	float2 texCoord : TEXCOORD0;
	float3 tangent : TEXCOORD1;
	float3 grabPos : TEXCOORD2;
	float3 viewDir : TEXCOORD3;
	float3 worldNormal : TEXCOORD4;
};

float4 _Color, _MainTex_ST;
float glitchIntensity;

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;

	input.position.z += sin(1.0f * glitchSpeed * 5 * input.position.y) * glitchIntensity;

	output.position = mul(float4(input.position, 1.0f), wvp);
	output.texCoord = mul(input.texCoord, textureMap.Sample(textureSampler));

	output.tangent = input.tangent;

	//Alpha mask coordinates
	output.grabPos = mul(float4(input.position, 1.0f), world).xyz;

	//Scroll Alpha mask uv
	output.grabPos.y += 1.0f * scrollSpeed;

	float3 worldPosition = mul(float4(input.position, 1.0f), world).xyz;

	output.worldNormal = mul(input.normal, (float3x3)world);
	output.viewDir = normalize(viewPosition - worldPosition);

	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	float dirVertex = (dot(input.grabPos, 1.0) + 1) / 2;

	float4 alphaColor = alphaTextureMap.Sample(textureSampler,  input.grabPos.xy * scale);
	float4 pixelColor = textureMap.Sample(textureSampler, input.texCoord);
	pixelColor.w = alphaColor.w;

	// Rim Light
	float rim = 1.0 - saturate(dot(input.viewDir, input.worldNormal));

	return pixelColor * _Color * (rim + glowIntensity);
}