struct VS_In
{
    float3 pos : Position;
};

struct VS_Out
{
    float4 pos : SV_Position;
    float3 normal : Normal;
};

cbuffer Transform : register( b0 )
{
    matrix model;
    matrix modelView;
    matrix modelViewProj;
    matrix modelRotation;
    matrix cameraRotation;
};

cbuffer GlobalParameter : register( b1 )
{
    float time;
}

static const int numWave = 8;

cbuffer WaveParameter : register( b2 )
{
    struct
    {
        float kx;
        float kz;
        float k;
        float w;
        float a;
        float phi;
        float _1;
        float _2;
    } wave[numWave];
};


VS_Out main( VS_In input )
{
    VS_Out output;
    float4 pos = float4( input.pos, 1.0f );
    
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
        float theta = dot( float2( wave[m].kx, wave[m].kz ), pos.xz ) - wave[m].w * time - wave[m].phi;
        float AC = wave[m].a * cos( theta );
        float AS = wave[m].a * sin( theta );
        float KxAS = wave[m].kx * AS;
        float KzAS = wave[m].kz * AS;
        float DkAC = AC / wave[m].k;
        float DkAS = AS / wave[m].k;
        float KxDkAS = wave[m].kx * DkAS;
        float KzDkAS = wave[m].kz * DkAS;
        float KxxDkAC = wave[m].kx * KxDkAS;
        float KzxDkAC = wave[m].kz * KxDkAS;
        float KzzDkAC = wave[m].kz * KzDkAS;
        
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
    
    float3 dpos_dalhpa = float3( 1.0f - S_KxxDkAC, -S_KzxDkAC, -S_KxAS );
    float3 dpos_dbeta = float3( -S_KzxDkAC, 1.0f - S_KzzDkAC, -S_KzAS );
    float3 normal = cross( dpos_dalhpa, dpos_dbeta );
   
    output.pos = mul( pos, modelViewProj );
    output.normal = (float3) mul( mul( float4( normal, 0.0f ), modelRotation ), cameraRotation );
    return output;
}