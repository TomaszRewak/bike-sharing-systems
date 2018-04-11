#pragma once

#include "../../CityBikes.Data/demand/demand-prediction.hpp"
#include "../../CityBikes.Data/demand/cumulative-demand-prediction.hpp"
#include "../../CityBikes.DataProcessing/flow/sorting.hpp"

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

		static double computeDistance(const Data::Demand::CumulativeDemandPrediction<Nodes>& demandA, const Data::Demand::CumulativeDemandPrediction<Nodes>& demandB, size_t window)
		{
			double distance = 0;

			size_t timeFrames = demandA.size();

			for (size_t node = 0; node < Nodes; node++)
			{
				double nodeDistance = 0;

				for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
				{
					double windowDistance = 0;

					double startDemandA = demandA[timeFrame][node];
					double startDemandB = demandB[timeFrame][node];

					for (size_t windowFrame = 0; windowFrame < window && timeFrame + windowFrame < timeFrames; windowFrame++)
					{
						double currentDemandA = demandA[timeFrame + windowFrame][node] - startDemandA;
						double currentDemandB = demandB[timeFrame + windowFrame][node] - startDemandB;

						windowDistance += std::pow(currentDemandA - currentDemandB, 2);
					}

					nodeDistance += windowDistance / window;
				}

				distance += nodeDistance / timeFrames;
			}

			return distance / Nodes;
		}
	};
}