// Reference : https://halisavakis.com/my-take-on-shaders-glitch-image-effect/

cbuffer ConstantBuffer : register(b0)
{
	matrix wvp;
}

cbuffer SettingsBuffer : register(b1)
{
	float chromoAmountX;
	float chromoAmountY;
	float glitchEffect;
	float desaturationAmount;
}

Texture2D textureMap : register(t0);
SamplerState textureSampler : register(s0);

float4 displacementAmount;
float rightStripes = 30.0f;
float rightStripesFill = 3.0f;
float leftStripes = 30.0f;
float leftStripesFill = 3.0f;
float wavyDisplayFreq = 5.0f;

float rand(float2 co)
{
	return frac(sin(dot(co, float2(12.9898, 78.233))) * 43758.5453);
}

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
	 float2 ChromAberrAmount = float2(chromoAmountX, chromoAmountY);

	 float4 displAmount = float4(0, 0, 0, 0);
	 float2 chromAberrAmount = float2(0, 0);
	 float rStripesFill = 0;
	 float lStripesFill = 0;

	 //Glitch control
	 if (frac(glitchEffect) < 0.8)
	 {
		 rStripesFill = lerp(0, rightStripesFill, frac(glitchEffect) * 2);
		 lStripesFill = lerp(0, leftStripesFill, frac(glitchEffect) * 2);
	 }
	 if (frac(glitchEffect) < 0.5)
	 {
		 chromAberrAmount = lerp(float2(0, 0), ChromAberrAmount.xy, frac(glitchEffect) * 2);
	 }
	 if (frac(glitchEffect) < 0.33)
	 {
		 displAmount = lerp(float4(0,0,0,0), displacementAmount, frac(glitchEffect) * 3);
	 }

	 //Stripes section
	 float stripesRight = floor(input.texCoord.y * rightStripes);
	 stripesRight = step(rStripesFill, rand(float2(stripesRight, stripesRight)));

	 float stripesLeft = floor(input.texCoord.y * leftStripes);
	 stripesLeft = step(lStripesFill, rand(float2(stripesLeft, stripesLeft)));
	 //Stripes section

	 float4 wavyDispl = lerp(float4(1,0,0,1), float4(0,1,0,1), (sin(input.texCoord.y * wavyDisplayFreq) + 1) / 2);

	 //Displacement section
	 float2 displUV = (displAmount.xy * stripesRight) - (displAmount.xy * stripesLeft);
	 displUV += (displAmount.zw * wavyDispl.r) - (displAmount.zw * wavyDispl.g);
	 //Displacement section

	 //Chromatic aberration section
	 float chromR = textureMap.Sample(textureSampler, input.texCoord + displUV + chromAberrAmount).r;
	 float chromG = textureMap.Sample(textureSampler, input.texCoord + displUV).g;
	 float chromB = textureMap.Sample(textureSampler, input.texCoord + displUV - chromAberrAmount).b;
	 //Chromatic aberration section

	 float4 finalCol = float4(chromR, chromG, chromB, 1);

	 return finalCol;
}
