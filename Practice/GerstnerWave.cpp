#include "GerstnerWave.h"

namespace wos = World::Object::Simulation;

void wos::GerstnerWave::SetWaveVector(float waveVecX, float waveVecZ) noexcept
{
    m_waveVecX    = waveVecX;
    m_waveVecZ    = waveVecZ;
    m_waveNumber  = std::sqrtf(waveVecX * waveVecX + waveVecZ * waveVecZ);
    m_waveAngFreq = 9.8f * m_waveNumber;
}

void wos::GerstnerWave::SetWaveAmplitude(float waveAmp) noexcept
{
    m_waveAmp = waveAmp;
}

void wos::GerstnerWave::SetWavePhase(float wavePhase) noexcept
{
    m_wavePhase = wavePhase;
}