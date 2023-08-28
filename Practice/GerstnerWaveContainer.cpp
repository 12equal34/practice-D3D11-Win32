#include "GerstnerWaveContainer.h"
#include <array>

namespace wos = World::Object::Simulation;

wos::GerstnerWaveContainer
wos::GerstnerWaveContainer::TestWaveGenerator::operator()() noexcept
{
    wos::GerstnerWaveContainer waveContainer = {};
    std::array                 init {
        std::make_unique<wos::GerstnerWave>(0.3f, -0.46f, 0.2f, 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.33f, 0.40f, 0.12f, 0.0f),
        std::make_unique<wos::GerstnerWave>(0.0f, 0.56f, 0.2f, 0.2f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.33f, 0.0f, 0.12f, 0.3f + 0.0f),
        std::make_unique<wos::GerstnerWave>(0.3f, -0.26f, 0.2f, 0.7f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.33f, 0.20f, 0.12f, 0.9f + 0.0f),
        std::make_unique<wos::GerstnerWave>(0.17f, 0.3f, 0.3f, -1.0f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.15f, -0.1f, 0.7f, 0.6f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.15f, 0.1f, 0.6f, 0.5f + 0.0f),
        std::make_unique<wos::GerstnerWave>(0.15f, -0.1f, 0.8f, 0.4f + 0.0f),
        std::make_unique<wos::GerstnerWave>(0.15f, 0.1f, 0.3f, 0.3f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.25f, -0.1f, 0.2f, 0.2f + 0.0f),
        std::make_unique<wos::GerstnerWave>(-0.15f, -0.25f, 0.4f, 0.1f + 0.0f),
    };
    for (auto& pWave : init) {
        waveContainer.Add(std::move(pWave));
    }
    return waveContainer;
}

void wos::GerstnerWaveContainer::Add(
    std::unique_ptr<GerstnerWave> wave) noexcept
{
    m_waves.push_back(std::move(wave));
}

size_t wos::GerstnerWaveContainer::GetNumOfWaves() const noexcept
{
    return m_waves.size();
}

size_t wos::GerstnerWaveContainer::GetByteWidth() const noexcept
{
    return sizeof(GerstnerWave) * GetNumOfWaves();
}

std::vector<ConstantBufferGerstnerWaveParameters>
wos::GerstnerWaveContainer::GetCbufDatas() noexcept
{
    std::vector<ConstantBufferGerstnerWaveParameters> cbufDatas;
    cbufDatas.reserve(GetNumOfWaves());

    for (const auto& pWave : m_waves) {
        cbufDatas.emplace_back(pWave->GetCbufData());
    }

    return cbufDatas;
}

void wos::GerstnerWaveContainer::SetNextPhases(float dt) noexcept
{
    for (const auto& pWave : m_waves) {
        pWave->SetWaveNextPhase(dt);
    }
}
