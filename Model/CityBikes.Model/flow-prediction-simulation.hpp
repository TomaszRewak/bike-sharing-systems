#pragma once

#include "../CityBikes.Data/flow/flow-target.hpp"
#include "../CityBikes.Data/demand/demand-prediction.hpp"
#include "../CityBikes.Data/supply/supply-prediction.hpp"
#include "../CityBikes.Data/fill-level/fill-level-prediction.hpp"

namespace CityBikes::Model
{
	template<size_t Nodes>
	class FlowPredictionSimulation
	{
	public:
		static Data::FillLevel::FillLevelPrediction<Nodes> predict(
			Data::FillLevel::FillLevelPredictionFrame<Nodes> currentFillLevel,
			const std::vector<Data::Flow::FlowTarget>& ongoingFlow,
			const Data::Supply::SupplyPrediction<Nodes>& ongoingSupplyPrediction,
			const Data::Demand::DemandPrediction<Nodes>& upcomingDemandPrediction,
			const Data::Supply::SupplyPrediction<Nodes>& upcomingSupplyPrediction,
			size_t startTimeFrame, size_t endTimeFrame
		)
		{
			Data::FillLevel::FillLevelPrediction<Nodes> prediction;
			std::vector<std::array<double, Nodes>> memory(endTimeFrame - startTimeFrame);

			for (const Data::Flow::FlowTarget& flow : ongoingFlow)
				for (size_t target = 0; target < Nodes; target++)
					for (const auto& supply : ongoingSupplyPrediction[flow.timeFrame][flow.node][target])
						if (flow.timeFrame + supply.first >= startTimeFrame && flow.timeFrame + supply.first < endTimeFrame)
							memory[flow.timeFrame + supply.first - startTimeFrame][target] += supply.second;

			for (size_t timeFrame = startTimeFrame; timeFrame < endTimeFrame; timeFrame++)
				for (size_t source = 0; source < Nodes; source++)
					memory[timeFrame - startTimeFrame][source] -= upcomingDemandPrediction[timeFrame][source];

			for (size_t timeFrame = startTimeFrame; timeFrame < endTimeFrame; timeFrame++)
				for (size_t source = 0; source < Nodes; source++)
					for (size_t target = 0; target < Nodes; target++)
						for (const auto& supply : upcomingSupplyPrediction[timeFrame][source][target])
							if (timeFrame + supply.first < endTimeFrame)
								memory[timeFrame + supply.first - startTimeFrame][target] += upcomingDemandPrediction[timeFrame][source] * supply.second;

			for (size_t timeFrame = startTimeFrame; timeFrame < endTimeFrame; timeFrame++)
			{
				for (size_t node = 0; node < Nodes; node++)
					currentFillLevel[node] += memory[timeFrame - startTimeFrame][node];
				prediction.push_back(currentFillLevel);
			}

			return prediction;
		}
	};
}