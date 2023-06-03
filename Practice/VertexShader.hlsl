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
    float time;
    float wave_amplitude;
    float wave_angular_frequency;
    float wave_phase;
    float2 wave_direction;
    float2 wave_number_vector;
    float wave_number;
};

VS_Out main( VS_In input )
{
    VS_Out output;
    float4 pos = float4( input.pos, 1.0f );
    float theta = dot( wave_number_vector, pos.xz ) - wave_angular_frequency * time - wave_phase;
    
    pos.xz = pos.xz - wave_number_vector / wave_number * wave_amplitude * sin( theta );
    pos.y = wave_amplitude * cos( theta );
   
    output.pos = mul( pos, modelViewProj );
    return output;
}