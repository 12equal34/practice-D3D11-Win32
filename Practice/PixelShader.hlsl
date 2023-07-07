struct VS_Out
{
    float4 pos : SV_Position;
    float3 normal : Normal;
};

cbuffer Light : register( b0 )
{
    float4 light_color;
    float3 light_direction;
};

float4 main( VS_Out input ) : SV_TARGET
{
    static float4 baseColor = float4( 0.7f, 0.7f, 1.0f, 1.0f );
    static float4 light_ambient = float4( 0.3f, 0.3f, 0.4f, 1.0f );
    float3 n = normalize( input.normal );
    float3 l = normalize( light_direction );
    
    float3 ambient = light_ambient.rgb * light_ambient.a * baseColor.rgb;
    float3 diffuse = float3( 0.0f, 0.0f, 0.0f );
    float n_dot_l = dot( n, l );
    if (n_dot_l > 0)
    {
        diffuse = light_color.rgb * light_color.a * n_dot_l * baseColor.rgb;
    }
    
    return float4( ambient + diffuse, baseColor.a );
}