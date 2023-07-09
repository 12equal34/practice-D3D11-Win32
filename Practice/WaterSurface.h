#pragma once
#include "Surface.h"
#include "GerstnerWaveContainer.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class WaterSurface : public Surface
{
public:
    WaterSurface(int numX, int numZ, float gridSize,
                 Simulation::GerstnerWaveContainer&& waveContainer)
        : Surface(numX, numZ, gridSize),
          m_waveContainer(std::move(waveContainer)),
          m_gerstnerWaveParamCbuf(
              static_cast<UINT>(m_waveContainer.GetByteWidth()))
    {
        m_gerstnerWaveParamCbuf.SetToVertexShader(2u);
    }

    void Bind() noexcept override;

    static Simulation::GerstnerWaveContainer
    TestWaveGenerator(float time) noexcept;

private:
    Simulation::GerstnerWaveContainer m_waveContainer;
    Hardware::DX::ConstantBuffer      m_gerstnerWaveParamCbuf;
};
}
