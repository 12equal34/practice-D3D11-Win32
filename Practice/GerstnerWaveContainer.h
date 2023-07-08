#pragma once
#include "GerstnerWave.h"
#include <vector>
#include <memory>

namespace World::Object::Simulation
{
class GerstnerWaveContainer
{
public:
    void Add(std::unique_ptr<GerstnerWave> wave) noexcept;

    size_t GetNumOfWaves() const noexcept;
    size_t GetByteWidth() const noexcept;

    std::vector<World::Object::Simulation::GerstnerWave>
    GetData() const noexcept;
private:
    std::vector<std::unique_ptr<GerstnerWave>> m_waves;
};
}
