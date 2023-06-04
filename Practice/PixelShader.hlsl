struct VS_Out
{
    float4 pos : SV_Position;
    float3 normal : Normal;
};

cbuffer Light : register( b0 )
{
    float4 light_ambient;
    float4 light_color;
    float3 light_direction;
};

float4 main( VS_Out input ) : SV_TARGET
{
    static float4 baseColor = float4( 0.4f, 0.4f, 1.0f, 1.0f );
    
    float3 normal = normalize( input.normal );
    float3 ambient = light_ambient.rgb * light_ambient.a * baseColor.rgb;
    
    float3 diffuse = (float3) 0;
    float n_dot_l = dot( light_direction, normal );
    if (n_dot_l > 0)
    {
        diffuse = light_color.rgb * light_color.a * n_dot_l * baseColor.rgb;
    }
    
    return float4( ambient + diffuse , baseColor.a );
}