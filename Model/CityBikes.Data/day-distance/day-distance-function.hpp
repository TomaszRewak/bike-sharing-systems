#pragma once

#include "../time/day.hpp"
#include "day-distance-value.hpp"

#include <map>

namespace CityBikes::Data::DayDistance
{
	template<size_t Nodes>
	struct DayDistanceFunction : public std::map<Time::Day, std::map<Time::Day, DayDistanceValue<Nodes>>>
	{ };
}