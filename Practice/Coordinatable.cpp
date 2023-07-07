#include "Coordinatable.h"

namespace woi = World::Object::Information;

woi::Coordinate& woi::Coordinatable::GetCoordinate() noexcept
{
    return const_cast<woi::Coordinate&>(
        static_cast<const Coordinatable*>(this)->GetCoordinate());
}

const woi::Coordinate& woi::Coordinatable::GetCoordinate() const noexcept
{
    return m_coordinate;
}