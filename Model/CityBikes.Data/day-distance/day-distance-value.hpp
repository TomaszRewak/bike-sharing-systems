#pragma once

#include <array>

namespace CityBikes::Data::DayDistance
{
	template<size_t Nodes>
	struct DayDistanceValue : public std::array<double, Nodes>
	{ };
}