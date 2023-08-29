#include "WaterSurface.h"
#include "ConstantBuffers.h"

World::Object::WaterSurface::WaterSurface(
    int numX, int numZ, float gridSize,
    Simulation::GerstnerWaveContainer&& waveContainer)
    : Surface(numX, numZ, gridSize),
      m_waveContainer(std::move(waveContainer)),
      m_gerstnerWaveParamsCB(sizeof(ConstantBufferGerstnerWaveParameters) *
                             (UINT)m_waveContainer.GetNumOfWaves())
{
    m_gerstnerWaveParamsCB.SetToVertexShader(4u);
}

void World::Object::WaterSurface::Simulate(float dt) noexcept
{
    m_waveContainer.SetNextPhases(dt);
}

void World::Object::WaterSurface::Bind()
{
    Surface::Bind();

    auto newWaveData = m_waveContainer.GetCbufDatas();
    m_gerstnerWaveParamsCB.Update(newWaveData.data());
}
