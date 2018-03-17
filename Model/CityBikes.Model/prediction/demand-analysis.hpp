#pragma once

#include "../../CityBikes.Data/demand/demand-prediction.hpp"
#include "../../CityBikes.DataProcessing/flow/sorting.hpp"

#include <algorithm>
#include <vector>

namespace CityBikes::Model::Prediction
{
	template<size_t Nodes>
	class DemandAnalysis
	{
	public:
		static Data::Demand::DemandPrediction<Nodes> computeDemand(const std::vector<Data::Flow::FlowAction>& actions, size_t timeFrames)
		{
			Data::Demand::DemandPrediction<Nodes> demandPrediction(timeFrames);

			for (const Data::Flow::FlowAction action : actions)
				demandPrediction[action.flowInstance.source.timeFrame][action.flowInstance.source.node] += action.actionProbability;

			return demandPrediction;
		}
	};
}