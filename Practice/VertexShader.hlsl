struct VS_In
{
    float3 pos : Position;
};

struct VS_Out
{
    float4 pos : SV_Position;
};

cbuffer Transform : register( b0 )
{
    matrix model;
    matrix modelView;
    matrix modelViewProj;
};

cbuffer WaveParameter : register( b1 )
{
    float time : Time;
    float wave_amplitude : WaveAmplitude;
    float wave_phase : WavePhase;
    float2 wave_direction : WaveDirection;
};

VS_Out main( VS_In input )
{
    VS_Out output;
    float4 vertexPos = float4( input.pos, 1.0f );
    vertexPos.y = wave_amplitude * sin( dot( wave_direction, input.pos.xz ) + time * wave_phase );
    output.pos = mul( vertexPos, modelViewProj );
    return output;
}