#pragma once

#include "../../CityBikes.Data/demand/demand-prediction.hpp"
#include "../../CityBikes.Data/demand/cumulative-demand-prediction.hpp"
#include "../../CityBikes.DataProcessing/flow/sorting.hpp"

#include <algorithm>
#include <vector>

namespace CityBikes::Model::Prediction
{
	template<size_t Nodes>
	class DistanceAnalysis
	{
	public:
		static std::array<double, Nodes> computeSimpleDistance(const Data::Demand::CumulativeDemandPrediction<Nodes>& demandA, const Data::Demand::CumulativeDemandPrediction<Nodes>& demandB)
		{
			std::array<double, Nodes> distances;

			size_t timeFrames = demandA.size();

			for (size_t node = 0; node < Nodes; node++)
			{
				double nodeDistance = 0;

				for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
				{
					double windowDistance = 0;

					double currentDemandA = demandA[timeFrame][node];
					double currentDemandB = demandB[timeFrame][node];

					nodeDistance += std::abs(currentDemandA - currentDemandB);
				}

				distances[node] = nodeDistance / timeFrames;
			}

			return distances;
		}

		static std::array<double, Nodes> computeDistance(const Data::Demand::CumulativeDemandPrediction<Nodes>& demandA, const Data::Demand::CumulativeDemandPrediction<Nodes>& demandB, size_t window)
		{
			std::array<double, Nodes> distances;

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

						windowDistance += std::abs(currentDemandA - currentDemandB);
					}

					nodeDistance += windowDistance / window;
				}

				distances[node] = nodeDistance / timeFrames;
			}

			return distances;
		}
	};
}