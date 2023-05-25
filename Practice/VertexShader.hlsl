struct VSOut
{
    float4 pos : SV_Position;
    float3 color : Color;
};

VSOut main( float2 pos : Position, float3 color : Color )
{
    VSOut vso;
    vso.pos = float4( pos, 0.0f, 1.0f );
    vso.color = float3( pos, 0.0f );
    return vso;
}