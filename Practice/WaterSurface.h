#pragma once
#include "Surface.h"
#include "GerstnerWaveContainer.h"
#include "ConstantBuffer.h"

namespace World::Object
{
class WaterSurface : public Surface
{
public:
    WaterSurface(int numXGrid, int numZGrid, float gridSize,
                 Simulation::GerstnerWaveContainer&& waveContainer);

    void Simulate(float dt) noexcept;

    void Bind() override;

private:
    Simulation::GerstnerWaveContainer m_waveContainer;
    Hardware::DX::ConstantBuffer      m_gerstnerWaveParamsCB;
};
}
