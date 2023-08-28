static const uint numWave = 8;

cbuffer GerstnerWaveParameters : register(b4)
{
    struct
    {
        float waveNumberX;
        float waveNumberZ;
        float amplitude;
        float phase;
    } wave[numWave];
};