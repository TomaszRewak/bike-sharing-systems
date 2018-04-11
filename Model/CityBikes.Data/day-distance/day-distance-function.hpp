#pragma once

#include "../time/day.hpp"

#include <map>

namespace CityBikes::Data::DayDistance
{
	struct DayDistanceFunction : public std::map<Time::Day, std::map<Time::Day, double>>
	{ };
}