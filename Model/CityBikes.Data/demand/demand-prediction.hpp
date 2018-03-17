#pragma once

#include <vector>

#include "demand-prediction-frame.hpp"

namespace CityBikes::Data::Demand
{
	template<size_t Nodes>
	struct DemandPrediction : public std::vector<DemandPredictionFrame<Nodes>>
	{ 
		DemandPrediction(size_t timeFrames):
			std::vector<DemandPredictionFrame<Nodes>>(timeFrames)
		{ }
	};
}