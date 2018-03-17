#pragma once

#include <array>

#include "supply-prediction-distribution.hpp"

namespace CityBikes::Data::Supply
{
	template<size_t Nodes>
	struct SupplyPredictionMatrix : public std::array<std::array<SupplyPredictionDistribution, Nodes>, Nodes>
	{ };
}