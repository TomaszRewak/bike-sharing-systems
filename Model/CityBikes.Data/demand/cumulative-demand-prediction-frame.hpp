#pragma once

#include <array>

namespace CityBikes::Data::Demand
{
	template<size_t Nodes>
	struct CumulativeDemandPredictionFrame : public std::array<double, Nodes>
	{ };
}