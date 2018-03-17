#pragma once

#include <array>

namespace CityBikes::Data::FillLevel
{
	template <size_t Nodes>
	struct FillLevelPredictionFrame : public std::array<double, Nodes>
	{ };
}