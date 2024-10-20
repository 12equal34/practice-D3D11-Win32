#ifndef PS_INPUT_STRUCTS_HLSLI
#define PS_INPUT_STRUCTS_HLSLI

struct PS_Input
{
    float4 ClipSpacePosition : SV_POSITION;
    float4 ToCameraNorm : POSITION0;
    float3 WorldNormalVector : NORMAL; // normal
    float4 WorldPosition : POSITION1;
    float4 ShadowPosition : TEXCOORD1;
    float2 UV : TEXCOORD0; // tex coord	
    float3 WorldTangentVector : TANGENT; // tangent
    float3 WorldBinormalVector : BINORMAL; // bi-tangent	
    float4 VertexColor : COLOR; // solid color (for debug)
    float4 MaterialEmissive : COLOR1; //vMaterialEmissive
    float4 MaterialDiffuse : COLOR2; //vMaterialDiffuse
};

#endif