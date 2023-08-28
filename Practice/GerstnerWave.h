#pragma once
#include "ConstantBuffers.h"

namespace World::Object::Simulation
{
class GerstnerWave
{
public:
    GerstnerWave(float waveNumberX, float waveNumberZ, float waveAmp,
                 float initWavePhase) noexcept;

    void SetWaveVector(float waveVecX, float waveVecZ) noexcept;
    void SetWaveAmplitude(float waveAmp) noexcept;
    void SetWaveNextPhase(float dt) noexcept;

    void ResetPhase(float initWavePhase) noexcept;

    ConstantBufferGerstnerWaveParameters GetCbufData() const noexcept;

private:
    float m_waveNumberX;
    float m_waveNumberZ;
    float m_waveNumber;
    float m_waveAngFreq;
    float m_waveAmp;
    float m_waveInitPhase;
    float m_wavePhase;
};
}