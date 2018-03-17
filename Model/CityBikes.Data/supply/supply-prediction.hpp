#pragma once

#include <vector>

#include "supply-prediction-matrix.hpp"

namespace CityBikes::Data::Supply
{
	template<size_t Nodes>
	struct SupplyPrediction : public std::vector<SupplyPredictionMatrix<Nodes>>
	{ 
		SupplyPrediction(size_t timeFrames):
			std::vector<SupplyPredictionMatrix<Nodes>>(timeFrames)
		{ }
	};
}