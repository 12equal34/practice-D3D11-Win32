#ifndef PS_MESH_BLINNPHONG_HLSL
#define PS_MESH_BLINNPHONG_HLSL

#include "PSInputStructs.hlsli"
#include "CommonFunctions.hlsli"
#include "ConstantBuffers.hlsli"
#include "CommonBuffers.hlsli"

float4 ComputeLitColor(float3 SurfacePosition,
					   float3 ToCamera/*Normalized*/,
					   float3 SurfaceNormalVector,
					   float4 Diffuse, float4 Specular, float Shininess, float3 Reflect);
float4 ComputeBlinnPhongLitColor(float3 N, float3 L, float3 H,
								 float4 LightColor, float4 Diffuse, float4 Specular, float Shininess);
float ShadowedLightStrength(float4 ShadowPosition);

float4 main(PS_Input Input) : SV_TARGET
{
	float3 WorldPosition = Input.WorldPosition.xyz;
	float3 WorldNormalVector;
	if (FlatShading)
    {
		float3 FlatPlaneNormalDir = cross(ddy(WorldPosition), ddx(WorldPosition));
		WorldNormalVector = normalize(FlatPlaneNormalDir);
    }
	else /*SmoothShading*/
    {
		WorldNormalVector = normalize(Input.WorldNormalVector);
    }
	if (HasNormalMap)
    {
		float3 SurfaceNormalVector = NormalMap.Sample(SurfaceSamplerState, Input.UV);
		SurfaceNormalVector = mad(2.f, SurfaceNormalVector, -1.f); // 2*BumpMap - 1, Mapping from (0,1) to (-1,1)
		
		if (AutoTangent)
        {
			float3 dp1 = ddx(WorldPosition);
			float3 dp2 = ddy(WorldPosition);
			float3x3 M = float3x3(dp1, dp2, cross(dp1, dp2));
			float2x3 InverseM = float2x3(cross(M[1], M[2]), cross(M[2], M[0]));
			float2x3 TB = mul(transpose(float2x2(ddx(Input.UV), ddy(Input.UV))), InverseM);
			float3 WorldTangentVector  = normalize(TB[0]);
			float3 WorldBinormalVector = normalize(TB[1]);
			float3x3 SurfaceToWorld = float3x3(WorldTangentVector, WorldBinormalVector, WorldNormalVector);
			WorldNormalVector = normalize(mul(SurfaceNormalVector, SurfaceToWorld));
        }
        else
        {
			float3 WorldTangentVector  = normalize(Input.WorldTangentVector);
			float3 WorldBinormalVector = normalize(Input.WorldBinormalVector);
			float3 WorldDeltaNormalVector = mad(SurfaceNormalVector.x, WorldTangentVector, SurfaceNormalVector.y * WorldBinormalVector);
			WorldNormalVector = normalize(WorldNormalVector + WorldDeltaNormalVector);
        }
    }
	
	float4 Diffuse = Input.MaterialDiffuse;
	if (HasDiffuseMap)
    {
		Diffuse *= DiffuseMap.Sample(SurfaceSamplerState, Input.UV);
    }
	Diffuse = lerp(Diffuse, Input.VertexColor, VertexColorBlending);
	
	float Alpha = 1.f;
	if (HasAlphaMap)
    {
		float4 AlphaColor = AlphaMap.Sample(SurfaceSamplerState, Input.UV);
		Alpha = AlphaColor.a;
		Diffuse.rgb *= AlphaColor.rgb;
    }
	
	float4 Specular = MaterialSpecular;
	float Shininess = MaterialShininess;
	float4 Reflect = MaterialReflect;
	if (IsBatched)
    {
		Specular = FloatToRGB(Input.VertexColor.z);
		Shininess = Input.VertexColor.x;
		Reflect = FloatToRGB(Input.VertexColor.w);
    }
	if (HasSpecularMap)
    {
		Specular *= SpecularMap.Sample(SurfaceSamplerState, Input.UV);
    }
	
	float4 LitColor = ComputeLitColor(WorldPosition,
								      normalize(Input.ToCameraNorm.xyz),
								      WorldNormalVector,
								      Diffuse, Specular, Shininess, Reflect.rgb);
	if (HasShadowMap)
    {
		if (ShadowMapRendering)
        {
			float3 ShadowDir = ShadowViewMatrix._m02_m12_m22;
			if (dot(ShadowDir, WorldNormalVector) > 0)
            {
				LitColor.rgb *= ShadowedLightStrength(Input.ShadowPosition);
            }
        }
    }
	
	float4 Emissive = Input.MaterialEmissive;
	if (HasEmissiveMap)
    {
		Emissive.rgb *= EmissiveMap.Sample(SurfaceSamplerState, Input.UV).rgb;
    }
	
	float4 Ambient = LightAmbient * MaterialAmbient;
	//TODO: SSAO
	
	LitColor.rgb += Emissive.rgb + Ambient.rgb;
	LitColor.a = Diffuse.a * Alpha;
	
	return saturate(LitColor);
}

float4 ComputeLitColor(float3 SurfacePosition,
					   float3 ToCamera/*Normalized*/,
					   float3 SurfaceNormalVector,
					   float4 Diffuse, float4 Specular, float Shininess, float3 Reflect)
{
    float4 LitColor = 0;
    for (int i = 0; i < NumOfLights; ++i)
    {
		if (Lights[i].LightType == 1/*Directional Light*/)
        {
			float3 ToLight = normalize(Lights[i].ReverseLookDir.xyz);
			float3 ToHalf = normalize(ToCamera + ToLight);
			LitColor += ComputeBlinnPhongLitColor(SurfaceNormalVector, ToLight, ToHalf,
												  Lights[i].Color, Diffuse, Specular, Shininess);
        }
		else if (Lights[i].LightType == 2/*Point Light*/)
        {
			float3 ToLight = Lights[i].Position.xyz - SurfacePosition;
			float SquaredDistToLight = dot(ToLight,ToLight);
			float DistToLight = sqrt(SquaredDistToLight);
			if (Lights[i].Attributes.w < DistToLight)
            {
				continue;
            }
			ToLight /= DistToLight;
			float3 ToHalf = normalize(ToCamera + ToLight);
			
			float3 Attr = Lights[i].Attributes.xyz * float3(1.f, DistToLight, SquaredDistToLight);
			float Attenuation = 1.f / (Attr.x + Attr.y + Attr.z);
			LitColor += Attenuation * ComputeBlinnPhongLitColor(SurfaceNormalVector, ToLight, ToHalf,
																Lights[i].Color, Diffuse, Specular, Shininess);
        }
		else if (Lights[i].LightType == 3/*Spot Light*/)
        {
			float3 ToLight = Lights[i].Position.xyz - SurfacePosition;
			float SquaredDistToLight = dot(ToLight,ToLight);
			float DistToLight = sqrt(SquaredDistToLight);
			if (Lights[i].Attributes.w < DistToLight)
            {
				continue;
            }
			ToLight /= DistToLight;
			float3 ToHalf = normalize(ToCamera + ToLight);
			float3 LookDir = normalize((-Lights[i].ReverseLookDir).xyz);
			
			float Effect = dot(ToLight, LookDir) - /*Cos(OuterAngle)*/Lights[i].SpotLightInfos.x;
			float DiffCosine = /*Cos(InnerAngle)*/Lights[i].SpotLightInfos.y - /*Cos(OuterAngle)*/Lights[i].SpotLightInfos.x;
			float Spot = pow( saturate(Effect/DiffCosine), /*Falloff*/Lights[i].SpotLightInfos.z );
			float3 Attr = Lights[i].Attributes.xyz * float3(1.f, DistToLight, SquaredDistToLight);
			float Attenuation = Spot / (Attr.x + Attr.y + Attr.z);
			LitColor += Attenuation * ComputeBlinnPhongLitColor(SurfaceNormalVector, ToLight, ToHalf,
																Lights[i].Color, Diffuse, Specular, Shininess);
        }
    }
	if (HasCubeMap)
    {
		float3 CameraToSurfaceDir = normalize(SurfacePosition - CameraWorldPosition);
		float3 ReflectionVector = reflect(CameraToSurfaceDir, SurfaceNormalVector);
		LitColor = (1.f - Reflect) * LitColor.rgb + Reflect * CubeMap.Sample(CubeSamplerState, ReflectionVector);
    }
	LitColor.a = Diffuse.a;
	return saturate(LitColor);
}


float ShadowedLightStrength(float4 ShadowPosition)
{
	ShadowPosition = ShadowPosition / ShadowPosition.w; // 정규화
	if (ShadowPosition.z < 0.f || ShadowPosition.z > 1.f)
    {
		return 1.f;
    }
	
	float2 Checking = abs(ShadowPosition).xy - float2(1,1);
	if (Checking.x > 0 || Checking.y > 0)
    {
		return 1.f;
    }
	
	ShadowPosition.x = mad(0.5f, ShadowPosition.x, 0.5f); // Mapping from (-1,1) to (0,1)
	ShadowPosition.y = mad(-0.5f, ShadowPosition.y, 0.5f); // Mapping from (-1,1) to (1,0)
	ShadowPosition.z -= ShadowMapInfo.z; // ShadowMap Bias, 그림자 경계에서 발생하는 Shadow Acne를 줄이기 위함.
	
	float ShadowFactor = 0.f;
	float2 Scale = 1.f / ShadowMapSize;
	[unroll] // PCF(Percentage Closer Filtering) on a 4X4 Grid texel neighboors
	for (float X = -1.5f; X <= 1.5f; X += 1.f) // 4개: -1.5, -0.5, 0.5, 1.5
    {
		for (float Y = -1.5f; Y <= 1.5f; Y += 1.f) // 4개: -1.5, -0.5, 0.5, 1.5
        {
			ShadowFactor += ShadowMap.SampleCmpLevelZero(ShadowSamplerState, mad(Scale, float2(X,Y), ShadowPosition.xy), ShadowPosition.z);
        }
    }
	ShadowFactor /= 16.f;
	
	return (1.f - ShadowMapInfo.x)/*고정값*/ + (ShadowFactor * ShadowMapInfo.x);
}

float4 ComputeBlinnPhongLitColor(float3 N, float3 L, float3 H,
								 float4 LightColor, float4 Diffuse, float4 Specular, float Shininess)
{
	float4 Coefficients = lit(dot(N,L), dot(N,H), Shininess);
	float4 DiffuseIntensity = Coefficients.y * Diffuse;
	float4 SpecularIntensity = min(Coefficients.z, Diffuse.w) * Specular;
	return LightColor * (DiffuseIntensity + SpecularIntensity);
}

#endif