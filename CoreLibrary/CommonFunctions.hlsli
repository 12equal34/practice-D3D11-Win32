#ifndef COMMON_FUNCTIONS_HLSLI
#define COMMON_FUNCTIONS_HLSLI

static const float4 encode = float4(16777216, 1.0 / 65536, 1.0 / 256, 1.0 / 255.0); 
float4 FloatToRGB(float v)
{
    uint vi = (uint) v;
    float4 c = (float4)0;
    c.r = vi >> 16;
    c.g = vi >> 8 & 0xFF;
    c.b = vi & 0xFF;
    c.rgb *= encode.w;
    c.a = 1;
    return c;
}

float RGBToFloat(float4 c)
{
    c *= 255;
    uint v = (uint) c.r << 16 + (uint) c.g << 8 + (uint) c.b;
    return v;
}

#endif