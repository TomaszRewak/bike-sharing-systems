#pragma once

#include "../../CityBikes.Model/flow-distribution-model.hpp"

namespace CityBikes::DataProcessing::Prediction
{
	class PredictionOffset
	{
	public:
		static double computeOffset(Model::FlowDistributionModel& modelA, Model::FlowDistributionModel& modelB)
		{
			double diff = 0;

			for (size_t node = 0; node < modelA.timeFrames[0].nodes.size(); node++)
				for (size_t timeFrame = 0; timeFrame < modelA.timeFrames.size(); timeFrame++)
					diff += std::pow(modelA.timeFrames[timeFrame].nodes[node].value - modelB.timeFrames[timeFrame].nodes[node].value, 2);

			diff /= modelA.timeFrames[0].nodes.size() * modelA.timeFrames.size();

			return diff;
		}
	};
}