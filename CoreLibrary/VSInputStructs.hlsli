#ifndef VS_INPUT_STRUCTS_HLSLI
#define VS_INPUT_STRUCTS_HLSLI

struct VS_Input
{
    float4 Position : POSITION;
    float3 NormalVector : NORMAL;
    float3 TangentVector : TANGENT;
    float3 BinormalVector : BINORMAL;
    float2 UV : TEXCOORD;
    float4 VertexColor : COLOR;
    float4 InstanceMatrixColumn0 : TEXCOORD1;
    float4 InstanceMatrixColumn1 : TEXCOORD2;
    float4 InstanceMatrixColumn2 : TEXCOORD3;
    float4 InstanceMatrixColumn3 : TEXCOORD4;
};

struct VS_InstancingInput
{
    float4 Position : POSITION;
    float3 NormalVector : NORMAL;
    float3 TangentVector : TANGENT;
    float3 BinormalVector : BINORMAL;
    float2 UV : TEXCOORD;
    float4 VertexColor : COLOR;
    float4 InstanceMatrixColumn0 : TEXCOORD1;
    float4 InstanceMatrixColumn1 : TEXCOORD2;
    float4 InstanceMatrixColumn2 : TEXCOORD3;
    float4 InstanceMatrixColumn3 : TEXCOORD4;
    float4 InstanceMaterialDiffuse : COLOR1;
    float4 InstanceMaterialEmissive : COLOR2;
    float2 UVOffset : TEXCOORD5;
};

#endif