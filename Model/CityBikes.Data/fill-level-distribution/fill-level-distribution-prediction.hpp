#pragma once

#include "fill-level-distribution-prediction-frame.hpp"
#include "../fill-level/fill-level-prediction.hpp"

namespace CityBikes::Data::FillLevelDistribution
{
	template<size_t Nodes>
	struct FillLevelDistributionPrediction : public std::vector<FillLevelDistributionPredictionFrame<Nodes>>
	{
		FillLevelDistributionPrediction(const FillLevel::FillLevelPrediction<Nodes>& model) :
			std::vector<FillLevelDistributionPredictionFrame<Nodes>>(model.size())
		{
			size_t timeFrames = model.size();

			for (size_t node = 0; node < Nodes; node++)
				(*this)[timeFrames - 1][node] = FillLevelDistributionPredictionNode(std::round(model[timeFrames - 1][node]));

			for (size_t timeFrame = timeFrames - 1; timeFrame > 0; timeFrame--)
				for (size_t node = 0; node < Nodes; node++)
					(*this)[timeFrame - 1][node] = FillLevelDistributionPredictionNode(std::round(model[timeFrame - 1][node]), (*this)[timeFrame][node]);
		}
	};
}