#include "GerstnerWaveContainer.h"

namespace wos = World::Object::Simulation;

void World::Object::Simulation::GerstnerWaveContainer::Add(
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

std::vector<wos::GerstnerWave> wos::GerstnerWaveContainer::GetData() const noexcept
{
    std::vector<GerstnerWave> waveData;
    waveData.reserve(GetNumOfWaves());
    for (const auto& pWave : m_waves) {
        waveData.emplace_back(*pWave);
    }
    return waveData;
}
