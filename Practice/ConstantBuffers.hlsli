Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);

cbuffer ConstantBufferNeverChanges : register(b0)
{
    float3 dirToDirectionalLight;
    float3 directionalLightColor;
}

cbuffer ConstantBufferChangeOnResize : register(b1)
{
    matrix projection;
};

cbuffer ConstantBufferChangesEveryFrame : register(b2)
{
    matrix view;
};

cbuffer ConstantBufferChangesEveryPrim : register(b3)
{
    matrix world;
    float3 meshColor;
    float3 diffuseColor;
    float3 specularColor;
    float specularExponent;
    float specularIntensity;
};

struct VertexShaderInput
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 textureUV : TEXCOORD0;
};

struct PixelShaderInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float2 textureUV : TEXCOORD0;
    float3 vertexToEye : TEXCOORD1;
};

struct SimpleVertexShaderInput
{
    float3 position : POSITION;
};

struct SimplePixelShaderInput
{
    float4 position : SV_Position;
    float3 vertexToEye : POSITION;
    float3 normal : NORMAL;
};


float3 CalculateDirectionalLight(const in float3 normal, const in float3 vertexToEye)
{
    float diffuseLuminance = saturate(dot(dirToDirectionalLight, normal));
    float3 diffuse = diffuseColor * diffuseLuminance;
 
    float NormalDotHalf = saturate(dot(
                normal, normalize(vertexToEye + dirToDirectionalLight)));
    float specularLuminance = pow(NormalDotHalf, specularExponent)
                               * specularIntensity;
    float3 specular = directionalLightColor * specularColor * specularLuminance;
    
    return diffuse * 0.5f + specular * 0.5f;
}