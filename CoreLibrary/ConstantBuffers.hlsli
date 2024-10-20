#ifndef CONSTANT_BUFFERS_HLSLI
#define CONSTANT_BUFFERS_HLSLI

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
    
    bool3 bParams;
    bool IsBatched = false;
    
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
    
    bool ShadowMapRendering = false;
    bool PhysicallyBasedRendering = false;
    bool FlatShading = false; // true¿Ã∏È SmoothShading
    bool AutoTangent;
    
    float4 DisplacementMapScaleMask = float4(0.f, 0.f, 0.f, 1.f);
    float4 UVTransformColumn1;
    float4 UVTransformColumn2;
    float VertexColorBlending;
}

#define NUM_LIGHTS 8
struct LightStruct
{
    int LightType;
    float3 pad0;
    float4 ReverseLookDir;
    float4 Position;
    float4 Attributes;
    float4 SpotLightInfos; // [outer angle, inner angle, falloff, free]
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
    float2 ShadowMapSize;
    bool HasShadowMap;
    float padding20;
    float4 ShadowMapInfo;
    float4x4 ShadowViewMatrix;
    float4x4 ShadowProjectionMatrix;
    float4x4 ShadowViewProjectionMatrix;
}

#endif