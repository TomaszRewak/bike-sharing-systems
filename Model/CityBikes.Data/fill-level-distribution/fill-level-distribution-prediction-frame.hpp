#pragma once

#include "fill-level-distribution-prediction-node.hpp"

#include <array>

namespace CityBikes::Data::FillLevelDistribution
{
	template<size_t Nodes>
	struct FillLevelDistributionPredictionFrame : public std::array<FillLevelDistributionPredictionNode, Nodes>
	{ };
}