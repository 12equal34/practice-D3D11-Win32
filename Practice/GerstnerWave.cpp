#include "GerstnerWave.h"
#include <cmath>
#include "numeric_utility_functions.h"

namespace wos = World::Object::Simulation;

wos::GerstnerWave::GerstnerWave(float waveNumberX, float waveNumberZ,
                                float waveAmp, float initWavePhase) noexcept
    : m_waveNumberX(waveNumberX),
      m_waveNumberZ(waveNumberZ),
      m_waveNumber(
          std::sqrtf(waveNumberX * waveNumberX + waveNumberZ * waveNumberZ)),
      m_waveAngFreq(9.8f * m_waveNumber),
      m_waveAmp(waveAmp),
      m_waveInitPhase(initWavePhase),
      m_wavePhase(initWavePhase)
{ }

void wos::GerstnerWave::SetWaveVector(float waveNumberX,
                                      float waveNumberZ) noexcept
{
    m_waveNumberX = waveNumberX;
    m_waveNumberZ = waveNumberZ;
    m_waveNumber =
        std::sqrtf(waveNumberX * waveNumberX + waveNumberZ * waveNumberZ);
    m_waveAngFreq = 9.8f * m_waveNumber;
}

void wos::GerstnerWave::SetWaveAmplitude(float waveAmp) noexcept
{
    m_waveAmp = waveAmp;
}

void wos::GerstnerWave::SetWaveNextPhase(float dt) noexcept
{
    m_wavePhase += m_waveAngFreq * dt;
    gp::math::NumericUtilityFunc::map_on_range(std::ref(m_wavePhase), 0,
                                               DirectX::XM_2PI);
}

void wos::GerstnerWave::ResetPhase(float initWavePhase) noexcept
{
    m_waveInitPhase = initWavePhase;
    m_wavePhase     = initWavePhase;
}

ConstantBufferGerstnerWaveParameters
wos::GerstnerWave::GetCbufData() const noexcept

{
    return ConstantBufferGerstnerWaveParameters {m_waveNumberX, m_waveNumberZ,
                                                 m_waveAmp, m_wavePhase};
};