struct VSOut
{
    float4 pos : SV_Position;
};

cbuffer Transform : register( b0 )
{
    matrix model;
    matrix modelView;
    matrix modelViewProj;
};

VSOut main( float3 pos : Position )
{
    VSOut vso;
    vso.pos = mul( float4( pos, 1.0f ), modelViewProj );
    return vso;
}