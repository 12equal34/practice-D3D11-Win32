struct VSOut
{
    float4 pos : SV_Position;
    float4 color : Color;
};

VSOut main( float2 pos : Position, uint4 color : Color )
{
    VSOut vso;
    vso.pos = float4( pos, 0.0f, 1.0f );
    vso.color = float4( color ) / 255.0f;
    return vso;
}