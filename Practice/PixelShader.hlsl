#include "ConstantBuffers.hlsli"
#include "HemisphericAmbientLight.hlsli"



float4 main(SimplePixelShaderInput input) : SV_TARGET
{
    input.vertexToEye = normalize(input.vertexToEye);
    input.normal = normalize(input.normal);
    
    // Calculate the ambient color
    float3 ambient = CalculateAmbientLight(input.normal, diffuseColor);
 
    // Calculate the directional light
    float3 directional = CalculateDirectionalLight(input.normal, input.vertexToEye);
    
    return float4(ambient + directional, 1.0f);
}