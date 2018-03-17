#pragma once

#include "fill-level-prediction-frame.hpp"

#include <vector>

namespace CityBikes::Data::FillLevel
{
	template<size_t Nodes>
	struct FillLevelPrediction : public std::vector<FillLevelPredictionFrame<Nodes>>
	{ };
}