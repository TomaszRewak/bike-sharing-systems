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
	class DemandProcessing
	{
	public:
		static std::vector<double> computeGlobalDemand(const Data::Demand::DemandPrediction<Nodes>& demand)
		{
			std::vector<double> globalDemand;

			for (const auto& frame : demand)
			{
				double frameDemand = 0;

				for (auto value : frame)
					frameDemand += value;

				globalDemand.push_back(frameDemand);
			}

			return globalDemand;
		}

		static Data::Demand::DemandPrediction<Nodes> computeDemand(std::vector<double> globalDemand, const Data::Demand::DemandPrediction<Nodes>& demandDistribution)
		{
			Data::Demand::DemandPrediction<Nodes> demand = demandDistribution;

			for (size_t frame = 0; frame < demand.size(); frame++)
			{
				double frameDemand = 0;

				for (auto value : demand[frame])
					frameDemand += value;

				if (frameDemand > 0)
					for (auto& value : demand[frame])
						value *= globalDemand[frame] / frameDemand;
				else
					for (auto& value : demand[frame])
						value = 0;
			}

			return demand;
		}
	};
}