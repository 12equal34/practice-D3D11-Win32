cbuffer Cbuf2
{
    float4 face_color[2];
};

float4 main( float4 pos : SV_Position, uint tid : SV_PrimitiveID) : SV_TARGET
{
    return face_color[tid %2];
}