#ifndef VS_MESH_DEFAULT_HLSL
#define VS_MESH_DEFAULT_HLSL

#include "VSInputStructs.hlsli"
#include "PSInputStructs.hlsli"
#include "CommonBuffers.hlsli"
#include "ConstantBuffers.hlsli"

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
        Output.WorldTangentVector  = normalize(mul(ModelTangentVector,  (float3x3) WorldMatrix));
        Output.WorldBinormalVector = normalize(mul(ModelBinormalVector, (float3x3) WorldMatrix));
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
    
    Output.ClipSpacePosition = mul(Output.WorldPosition, ViewProjectionMatrix);
    Output.VertexColor = Input.VertexColor;
    
    return Output;
}

#endif