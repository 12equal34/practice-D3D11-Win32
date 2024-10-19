cbuffer CB_Observer : register(b0)
{
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
    float4x4 ViewProjectionMatrix;
    float4 CameraFrustum; // [FOV,AspectRatio,Near,Far]
    float4 Viewport;      // [Width,Height,1/Width,1/Height]
    float3 CameraWorldPosition;
    float WorldTimeSeconds;
}

cbuffer CB_Mesh : register(b3)
{
    float4x4 WorldMatrix;
    
    bool IsNormalVectorReverse = false;
    bool HasInstances = false;
    bool HasInstanceParams = false;
    bool HasBones = false;
    
    float4 MaterialDiffuse = 0.5f;
    float4 MaterialAmbient = 0.25f;
    float4 MaterialEmissive = 0.0f;
    float4 MaterialSpecular = 0.0f;
    float4 MaterialReflect = 0.0f;
    
    bool HasDiffuseMap = false;
    bool HasNormalMap = false;
    bool HasCubeMap = false;
    bool HasEmissiveMap = false;
    
    bool HasAlphaMap = false;
    bool HasSpecularMap;
    bool HasDisplacementMap = false;
    float MaterialShininess = 1.0f;
    
    float4 DisplacementMapScaleMask = float4(0.f, 0.f, 0.f, 1.f);
    float4 UVTransformColumn1;
    float4 UVTransformColumn2;
}

#define NUM_LIGHTS 8

struct LightStruct
{
    float4 Direction;
    float4 Position;
    float4 Attribute;
    float4 Color;
    float4x4 ViewMatrix;
    float4x4 ProjectionMatrix;
};

cbuffer CB_Light : register(b1)
{
    LightStruct Lights[NUM_LIGHTS];
    float4 LightAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);
    int NumOfLights;
    bool HasEnvironmentMap;
    int NumOfEnvironmentMapMipLevels;
    float padding10;
}

cbuffer CB_Shadow : register(b2)
{
    float2 ShaderMapSize;
    bool HasShadowMap;
    float padding20;
    float4 ShadowMapInfo;
    float4x4 ShadowViewMatrix;
    float4x4 ShadowProjectionMatrix;
    float4x4 ShadowViewProjectionMatrix;
}

struct VS_Input
{
    float4 Position : POSITION;
    float3 NormalVector : NORMAL;
    float3 TangentVector : TANGENT;
    float3 BinormalVector : BINORMAL;
    float2 UV : TEXCOORD;
    float4 SolidColor : COLOR;
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
    float4 SolidColor : COLOR;
    float4 InstanceMatrixColumn0 : TEXCOORD1;
    float4 InstanceMatrixColumn1 : TEXCOORD2;
    float4 InstanceMatrixColumn2 : TEXCOORD3;
    float4 InstanceMatrixColumn3 : TEXCOORD4;
    float4 InstanceMaterialDiffuse : COLOR1;
    float4 InstanceMaterialEmissive : COLOR2;
    float2 UVOffset : TEXCOORD5;
};

struct PS_Input
{
    float4 Position : SV_POSITION;
    float4 ToCameraNorm : POSITION0;
    float3 WorldNormalVector : NORMAL; // normal
    float4 WorldPosition : POSITION1;
    float4 ShadowPosition : TEXCOORD1;
    float2 UV : TEXCOORD0; // tex coord	
    float3 TangentVector : TANGENT; // tangent
    float3 BinormalVector : BINORMAL; // bi-tangent	
    float4 SolidColor : COLOR; // solid color (for debug)
    float4 MaterialEmissive : COLOR1; //vMaterialEmissive
    float4 MaterialDiffuse : COLOR2; //vMaterialDiffuse
};

Texture2D<float4> DiffuseMap : register(t0);
Texture2D<float3> NormalMap : register(t1);
Texture2D<float4> AlphaMap : register(t2);
Texture2D<float4> SpecularMap : register(t3);
Texture2D<float3> DisplacementMap : register(t4);
Texture2D<float3> EmissiveMap : register(t5);

SamplerState DisplacementSamplerState : register(s0);
#if defined(INSTANCING_PARAMS)
PS_Input main(VS_InstancingInput Input)
#else
PS_Input main(VS_Input Input)
#endif
{
    PS_Input Output = (PS_Input) 0;
    
    // 모델 좌표에서의 정점 정보를 계산한다.
    float4 ModelPosition     = Input.Position;
    float3 ModelNormalVector = Input.NormalVector;
    if (IsNormalVectorReverse)
    {
        ModelNormalVector = -ModelNormalVector;
    }
    float3 ModelTangentVector  = Input.TangentVector;
    float3 ModelBinormalVector = Input.BinormalVector;
    if (HasInstances)
    {
        matrix InstanceMatrix =
        {
            Input.InstanceMatrixColumn0,
            Input.InstanceMatrixColumn1,
            Input.InstanceMatrixColumn2,
            Input.InstanceMatrixColumn3
        };
        ModelPosition     = mul(ModelPosition, InstanceMatrix);
        ModelNormalVector = mul(ModelNormalVector, (float3x3) InstanceMatrix);
        if (HasNormalMap)
        {
            ModelTangentVector  = mul(ModelTangentVector,  (float3x3) InstanceMatrix);
            ModelBinormalVector = mul(ModelBinormalVector, (float3x3) InstanceMatrix);
        }
    }
    
    // 월드 좌표에서의 정점 정보를 계산한다.
    Output.WorldPosition = mul(ModelPosition, WorldMatrix);
    Output.WorldNormalVector = normalize(mul(ModelNormalVector, (float3x3) WorldMatrix));
    if (HasNormalMap)
    {
        Output.TangentVector  = normalize(mul(ModelTangentVector,  (float3x3) WorldMatrix));
        Output.BinormalVector = normalize(mul(ModelBinormalVector, (float3x3) WorldMatrix));
    }
    
    // 월드 좌표에서의 변위 벡터(정점 -> 카메라)를 구한다.
    float3 ToCamera = CameraWorldPosition - Output.WorldPosition.xyz;
    Output.ToCameraNorm = float4(normalize(ToCamera), length(ToCamera));
    
    // UV 좌표를 알맞게 변환시키고 재질 특성값을 입력한다.
    Output.UV = mul(float4(Input.UV, 0.f, 1.f), float4x2(UVTransformColumn1, UVTransformColumn2)).xy;
#if defined(INSTANCING_PARAMS)
    if (HasInstanceParams)
    {
        Output.UV += Input.UVOffset;
        Output.MaterialDiffuse  = Input.InstanceMaterialDiffuse;
        Output.MaterialEmissive = Input.InstanceMaterialEmissive;
    }
    else
    {
        Output.MaterialDiffuse  = MaterialDiffuse;
        Output.MaterialEmissive = MaterialEmissive;
    }
#else
    Output.MaterialDiffuse  = MaterialDiffuse;
    Output.MaterialEmissive = MaterialEmissive;
#endif
    
    // 변위 맵을 적용하여 재질의 입체적 디테일을 더한다.
    if (HasDisplacementMap)
    {
        const float MipInterval = 20.f;
        float MipLevel = clamp((Output.ToCameraNorm.w - MipInterval) / MipInterval, 0, 6);
        float3 Height = DisplacementMap.SampleLevel(DisplacementSamplerState, Output.UV, MipLevel);
        Output.WorldPosition.xyz += Output.WorldNormalVector * mul(Height, DisplacementMapScaleMask.xyz);
    }
    
    // 그림자의 위치를 계산한다.
    if (HasShadowMap)
    {
        Output.ShadowPosition = mul(Output.WorldPosition, ShadowViewProjectionMatrix);
    }
    
    Output.Position = mul(Output.WorldPosition, ViewProjectionMatrix);
    Output.SolidColor = Input.SolidColor;
    
    return Output;
}