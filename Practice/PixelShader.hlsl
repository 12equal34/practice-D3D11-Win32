// we have to match order between this parameters and the outputs of vertex shader.
float4 main( float4 pos : SV_Position, float4 color : Color ) : SV_TARGET
{
    return float4( color );
}