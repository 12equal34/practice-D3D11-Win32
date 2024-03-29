#pragma once
#include "Coordinate.h"

namespace World::Object::Information
{
class Coordinatable
{
public:
	Information::Coordinate& GetCoordinate() noexcept;
	const Information::Coordinate& GetCoordinate() const noexcept;
protected:
	Information::Coordinate m_coord;
};
}