#pragma once

#include "../../CityBikes.Data/demand/demand-prediction.hpp"
#include "../../CityBikes.Data/demand/cumulative-demand-prediction.hpp"
#include "../../CityBikes.DataProcessing/flow/sorting.hpp"
#include "../../CityBikes.Data/time/day.hpp"

#include <algorithm>
#include <vector>

namespace CityBikes::Model::Prediction
{
	template<size_t Nodes>
	class DemandAnalysis
	{
	public:
		static Data::Demand::DemandPrediction<Nodes> computeDemand(const std::vector<Data::Flow::FlowAction>& flowActions, size_t timeFrames)
		{
			Data::Demand::DemandPrediction<Nodes> demandPrediction(timeFrames);

			for (const Data::Flow::FlowAction flowAction : flowActions)
				demandPrediction[flowAction.flowInstance.source.timeFrame][flowAction.flowInstance.source.node] += flowAction.actionProbability;

			return demandPrediction;
		}

		static Data::Demand::CumulativeDemandPrediction<Nodes> computeCumulativeDemand(const std::vector<Data::Flow::FlowAction>& flowActions, size_t timeFrames)
		{
			Data::Demand::DemandPrediction<Nodes> demandPrediction = computeDemand(flowActions, timeFrames);

			Data::Demand::CumulativeDemandPrediction<Nodes> cumulativeDemandPrediction(timeFrames);

			for (size_t node = 0; node < Nodes; node++)
				cumulativeDemandPrediction[0][node] = demandPrediction[0][node];

			for (size_t timeFrame = 1; timeFrame < timeFrames; timeFrame++)
				for (size_t node = 0; node < Nodes; node++)
					cumulativeDemandPrediction[timeFrame][node] = cumulativeDemandPrediction[timeFrame - 1][node] + demandPrediction[timeFrame][node];

			return cumulativeDemandPrediction;
		}

		static Data::Demand::CumulativeDemandPrediction<Nodes> computeCumulativeDemand(const std::vector<Data::Flow::FlowInstance>& flowInstances, size_t timeFrames)
		{
			std::vector<Data::Flow::FlowAction> flowActions;
			for (const Data::Flow::FlowInstance& flowInstance : flowInstances)
				flowActions.push_back(Data::Flow::FlowAction(flowInstance, 1));

			return computeCumulativeDemand(flowActions, timeFrames);
		}

		static void fixCumulativeDemandPrediction(Data::Demand::CumulativeDemandPrediction<Nodes>& cumulativeDemandPrediction)
		{
			for (size_t timeFrame = 1; timeFrame < cumulativeDemandPrediction.size(); timeFrame++)
				for (size_t node = 0; node < Nodes; node++)
					cumulativeDemandPrediction[timeFrame][node] = std::max(
						cumulativeDemandPrediction[timeFrame][node], 
						cumulativeDemandPrediction[timeFrame - 1][node]);
		}

		static Data::Demand::CumulativeDemandPrediction<Nodes> accumulateDemand(Data::Demand::DemandPrediction<Nodes> demandPrediction)
		{
			Data::Demand::CumulativeDemandPrediction<Nodes> cumulativeDemandPrediction(demandPrediction.size());

			for (size_t node = 0; node < Nodes; node++)
				cumulativeDemandPrediction[0][node] = demandPrediction[0][node];

			for (size_t timeFrame = 1; timeFrame < demandPrediction.size(); timeFrame++)
				for (size_t node = 0; node < Nodes; node++)
					cumulativeDemandPrediction[timeFrame][node] = cumulativeDemandPrediction[timeFrame - 1][node] + demandPrediction[timeFrame][node];

			return cumulativeDemandPrediction;
		}

		static Data::Demand::DemandPrediction<Nodes> decumulateDemand(Data::Demand::CumulativeDemandPrediction<Nodes> cumulativeDemandPrediction)
		{
			Data::Demand::DemandPrediction<Nodes> demandPrediction(cumulativeDemandPrediction.size());

			fixCumulativeDemandPrediction(cumulativeDemandPrediction);

			for (size_t node = 0; node < Nodes; node++)
				demandPrediction[0][node] = std::max(cumulativeDemandPrediction[0][node], 0.);

			for (size_t timeFrame = 1; timeFrame < demandPrediction.size(); timeFrame++)
				for (size_t node = 0; node < Nodes; node++)
					demandPrediction[timeFrame][node] = cumulativeDemandPrediction[timeFrame][node] - cumulativeDemandPrediction[timeFrame - 1][node];

			return demandPrediction;
		}
	};
}