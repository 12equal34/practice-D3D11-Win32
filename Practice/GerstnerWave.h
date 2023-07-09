#pragma once
#include <cmath>

namespace World::Object::Simulation
{
class GerstnerWave
{
public:
    GerstnerWave(float waveVecX, float waveVecZ, float waveAmp,
                 float wavePhase) noexcept
        : m_waveVecX(waveVecX),
          m_waveVecZ(waveVecZ),
          m_waveNumber(std::sqrtf(waveVecX * waveVecX + waveVecZ * waveVecZ)),
          m_waveAngFreq(9.8f * m_waveNumber),
          m_waveAmp(waveAmp),
          m_wavePhase(wavePhase),
          _1(NULL),
          _2(NULL)
    { }

    void SetWaveVector(float waveVecX, float waveVecZ) noexcept;
    void SetWaveAmplitude(float waveAmp) noexcept;
    void SetWavePhase(float wavePhase) noexcept;

private:
    float m_waveVecX;
    float m_waveVecZ;
    float m_waveNumber;
    float m_waveAngFreq;
    float m_waveAmp;
    float m_wavePhase;
    float _1,_2;
};
}