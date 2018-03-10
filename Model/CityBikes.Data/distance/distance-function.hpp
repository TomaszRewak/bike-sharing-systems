#pragma once

#include "../common/day.hpp"

namespace CityBikes::Data::Distance
{
	struct DistanceFunction : public std::map<Common::Day, float>
	{ };
}