#include "ConstantBuffers.hlsli"
#include "GerstnerWaveParameters.hlsli"

SimplePixelShaderInput main(SimpleVertexShaderInput input)
{
    SimplePixelShaderInput output = (SimplePixelShaderInput) 0;
    float4 pos = input.position;
    
    float S_KxDkAS = 0.0f;
    float S_KzDkAS = 0.0f;
    float S_AC = 0.0f;
    float S_KxxDkAC = 0.0f;
    float S_KzxDkAC = 0.0f;
    float S_KzzDkAC = 0.0f;
    float S_KxAS = 0.0f;
    float S_KzAS = 0.0f;
    for (int m = 0; m < numWave; ++m)
    {
        float a = wave[m].amplitude;
        float phi = wave[m].phase;
        
        float kx = wave[m].waveNumberX;
        float kz = wave[m].waveNumberZ;
        float k = sqrt(kx * kx + kz * kz);
        
        float theta = dot(float2(kx, kz), pos.xz) - phi;
        float AC = a * cos(theta);
        float AS = a * sin(theta);
        float KxAS = kx * AS;
        float KzAS = kz * AS;
        float DkAC = AC / k;
        float DkAS = AS / k;
        float KxDkAS = kx * DkAS;
        float KzDkAS = kz * DkAS;
        float KxxDkAC = kx * KxDkAS;
        float KzxDkAC = kz * KxDkAS;
        float KzzDkAC = kz * KzDkAS;
        
        S_KxDkAS += KxDkAS;
        S_KzDkAS += KzDkAS;
        S_AC += AC;
        S_KxxDkAC += KxxDkAC;
        S_KzxDkAC += KzxDkAC;
        S_KzzDkAC += KzzDkAC;
        S_KxAS += KxAS;
        S_KzAS += KzAS;
    }
    pos.x = pos.x - S_KxDkAS;
    pos.z = pos.z - S_KzDkAS;
    pos.y = pos.y + S_AC;
    
    float3 dpos_dalhpa = float3(1.0f - S_KxxDkAC, -S_KzxDkAC, -S_KxAS);
    float3 dpos_dbeta = float3(-S_KzxDkAC, 1.0f - S_KzzDkAC, -S_KzAS);
    float3 normal = cross(dpos_dalhpa, dpos_dbeta);
   
    output.position = mul(mul(mul(pos, world), view), projection);
    // output.textureUV = input.textureUV;
    output.normal = normalize(mul(mul(normal, (float3x3) world), (float3x3) view));
    output.vertexToEye = -mul(mul(pos, world), view).xyz;
    
    return output;
}