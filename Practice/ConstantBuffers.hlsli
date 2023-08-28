Texture2D diffuseTexture : register(t0);
SamplerState linearSampler : register(s0);

cbuffer ConstantBufferNeverChanges : register(b0)
{
    float3 dirToDirectionalLight : packoffset(c0);
    float4 directionalLightColor : packoffset(c1);
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
    float4 meshColor;
    float4 diffuseColor;
    float4 specularColor;
    //float specularExponent;
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
    float4 position : POSITION;
};

struct SimplePixelShaderInput
{
    float4 position : SV_Position;
    float3 normal : NORMAL;
    float3 vertexToEye : TEXCOORD1;
};