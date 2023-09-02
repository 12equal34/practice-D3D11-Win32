cbuffer HemisphericAmbientLight : register(b4)
{
    float3 ambientLightBaseColor;
    float3 ambientLightColorRange;
}

float3 CalculateAmbientLight(float3 normal, float3 color)
{
    float up = normal.y * 0.5f + 0.5f;
    float3 ambientLightColor = ambientLightBaseColor + up * ambientLightColorRange;
    return ambientLightColor * color;
}