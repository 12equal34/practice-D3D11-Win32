#include "Coordinatable.h"

using namespace World::Object;

Information::Coordinate& Coordinatable::GetCoordinate() noexcept
{
    return const_cast<Information::Coordinate&>(
        static_cast<const Coordinatable*>(this)->GetCoordinate());
}

const Information::Coordinate& Coordinatable::GetCoordinate() const noexcept
{
    return m_coordinate;
}