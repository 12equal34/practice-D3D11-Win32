#include "WaterSurface.h"

#include <array>

namespace WO = World::Object;

void WO::WaterSurface::Bind() noexcept
{
    Surface::Bind();

    auto waveData = m_waveContainer.GetData();
    m_gerstnerWaveParamCbuf.Update(waveData.data());
}

WO::Simulation::GerstnerWaveContainer
WO::WaterSurface::TestWaveGenerator(float time) noexcept
{
    using namespace WO::Simulation;
    using std::make_unique;

    GerstnerWaveContainer waveContainer;
    std::array            init {
        make_unique<GerstnerWave>(0.3f, -0.46f, 0.2f, time),
        make_unique<GerstnerWave>(-0.33f, 0.40f, 0.12f, time),
        make_unique<GerstnerWave>(0.0f, 0.56f, 0.2f, 0.2f + time),
        make_unique<GerstnerWave>(-0.33f, 0.0f, 0.12f, 0.3f + time),
        make_unique<GerstnerWave>(0.3f, -0.26f, 0.2f, 0.7f + time),
        make_unique<GerstnerWave>(-0.33f, 0.20f, 0.12f, 0.9f + time),
        make_unique<GerstnerWave>(0.17f, 0.3f, 0.3f, -1.0f + time),
        make_unique<GerstnerWave>(-0.15f, -0.1f, 0.7f, 0.6f + time),
        make_unique<GerstnerWave>(-0.15f, 0.1f, 0.6f, 0.5f + time),
        make_unique<GerstnerWave>(0.15f, -0.1f, 0.8f, 0.4f + time),
        make_unique<GerstnerWave>(0.15f, 0.1f, 0.3f, 0.3f + time),
        make_unique<GerstnerWave>(-0.25f, -0.1f, 0.2f, 0.2f + time),
        make_unique<GerstnerWave>(-0.15f, -0.25f, 0.4f, 0.1f + time),
    };
    for (auto& pWave : init) {
        waveContainer.Add(std::move(pWave));
    }
    return waveContainer;
}
