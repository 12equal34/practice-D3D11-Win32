#pragma once
#include <vector>
#include "IBindable.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Coordinatable.h"
#include <memory>

namespace World::Object
{
class Surface : public Hardware::DX::IBindable,
                public Information::Coordinatable
{
public:
    using IndexType = unsigned short;

    struct VertexType {
        float x, y, z;
    };

public:
    Surface(int numX, int numZ);
    std::unique_ptr<Hardware::DX::VertexBuffer> GetVertexBuffer(float gridSize);
    std::unique_ptr<Hardware::DX::IndexBuffer>  GetIndexBuffer();
    UINT                                        GetIndexCount() const noexcept;
    void                                        Bind() noexcept override;
protected:
    std::vector<std::unique_ptr<Hardware::DX::IBindable>> m_bindings;

    UINT                            m_numIndex;
    int                             m_nx;
    int                             m_nz;
    std::vector<std::vector<float>> m_H;
};
}