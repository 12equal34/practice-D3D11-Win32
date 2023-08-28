#pragma once
#include <vector>
#include <memory>
#include "GerstnerWave.h"
#include "ConstantBuffers.h"

namespace World::Object::Simulation
{
class GerstnerWaveContainer
{
public:
    class TestWaveGenerator
    {
    public:
        GerstnerWaveContainer operator()() noexcept;
    };
public:
    void Add(std::unique_ptr<GerstnerWave> wave) noexcept;

    size_t GetNumOfWaves() const noexcept;
    size_t GetByteWidth() const noexcept;

    std::vector<ConstantBufferGerstnerWaveParameters> GetCbufDatas() noexcept;

    void SetNextPhases(float dt) noexcept;
private:
    std::vector<std::unique_ptr<GerstnerWave>> m_waves;
};
}
