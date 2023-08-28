#include "ConstantBuffers.hlsli"
#include "HemisphericAmbientLight.hlsli"

float4 main(SimplePixelShaderInput input) : SV_TARGET
{
    input.vertexToEye = normalize(input.vertexToEye);
    
    float specularExponent = 1.0f;
    
    float diffuseLuminance =
        max(0.0f, dot(input.normal, dirToDirectionalLight));
    float specularLuminance =
        pow(max(0.0f, dot(input.normal, normalize(input.vertexToEye + dirToDirectionalLight))), specularExponent);
    
    float4 diffuse = diffuseColor * diffuseLuminance;
    float4 specular = specularColor * specularLuminance;
    float4 ambient = CalculateAmbientLight(input.normal, diffuseColor);
    
    return ambient + diffuse * 0.5f + specular * 0.5f;
}