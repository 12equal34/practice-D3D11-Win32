cbuffer HemisphericAmbientLight : register(b4)
{
    float4 ambientLightBaseColor;
    float4 ambientLightColorRange;
}

float4 CalculateAmbientLight(float3 normal, float4 color)
{
    float up = normal.y * 0.5f + 0.5f;
    float4 ambientLightColor = ambientLightBaseColor + up * ambientLightColorRange;
    return ambientLightColor * color;
}