#pragma once

#include <vector>

#include "cumulative-demand-prediction-frame.hpp"

namespace CityBikes::Data::Demand
{
	template<size_t Nodes>
	struct CumulativeDemandPrediction : public std::vector<CumulativeDemandPredictionFrame<Nodes>>
	{
		CumulativeDemandPrediction(size_t timeFrames) :
			std::vector<CumulativeDemandPredictionFrame<Nodes>>(timeFrames)
		{ }
	};
}