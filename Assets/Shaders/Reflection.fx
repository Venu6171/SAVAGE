// Reference: https://www.rastertek.com/tertut16.html

cbuffer ConstantBuffer : register(b0)
{
    matrix worldMatrix;
    matrix wvp;
}

cbuffer SettingsBuffer : register(b1)
{
	float4 clipPlane;
};

cbuffer LightBuffer : register(b2)
{
    float3 lightDirection;
    float4 lightAmbient;
    float4 lightDiffuse;
    float4 lightSpecular;
}

Texture2D colorTexture : register(t0);
Texture2D normalTexture : register(t1);
SamplerState textureSampler : register(s0);

struct VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 color : COLOR;
    float clip : SV_ClipDistance0;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(float4(input.position, 1.0f), wvp);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.texCoord;

    // Calculate the normal vector against the world matrix only and then normalize the final value.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = normalize(output.normal);

    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.binormal = mul(input.binormal, (float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);

    // Send the color map color into the pixel shader.	
    output.color = input.color;

    // Set the clipping plane.
    output.clip = dot(mul(float4(input.position, 1.0f), worldMatrix), clipPlane);

    return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
    float3 lightDir;
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float lightIntensity;
    float4 color;


    // Invert the light direction for calculations.
    lightDir = -lightDirection;

    // Sample the color texture.
    textureColor = colorTexture.Sample(textureSampler, input.texCoord);

    // Combine the color map value into the texture color.
    textureColor = saturate(input.color * textureColor * 1.0f);

    // Calculate the bump map using the normal map.
    bumpMap = normalTexture.Sample(textureSampler, input.texCoord);
    bumpMap = (bumpMap * 2.0f) - 1.0f;
    bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
    bumpNormal = normalize(bumpNormal);
    lightIntensity = saturate(dot(bumpNormal, lightDir));

    // Calculate the first bump mapped pixel color.
    color = saturate(lightDiffuseColor * lightIntensity);
    color = color * textureColor;

    return color;
}