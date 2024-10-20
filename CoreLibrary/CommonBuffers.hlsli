#ifndef COMMON_BUFFERS_HLSLI
#define COMMON_BUFFERS_HLSLI

Texture2D<float4> DiffuseMap : register(t0);
Texture2D<float3> NormalMap : register(t1);
Texture2D<float4> AlphaMap : register(t2);
Texture2D<float4> SpecularMap : register(t3);
Texture2D<float3> DisplacementMap : register(t4);
Texture2D<float3> EmissiveMap : register(t5);

Texture2D<float> ShadowMap : register(t6);
TextureCube<float> CubeMap : register(t7);

SamplerState SurfaceSamplerState : register(s0);
SamplerState DisplacementSamplerState : register(s1);
SamplerState CubeSamplerState : register(s4);
SamplerComparisonState ShadowSamplerState : register(s5);
#endif