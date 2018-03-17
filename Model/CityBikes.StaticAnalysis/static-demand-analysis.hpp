#pragma once

#include "configuration/demand-analysis-flow-configuration.hpp"

#include <array>

namespace CityBikes::StaticAnalysis
{
	template<size_t Nodes>
	class StaticDemandAnalysis
	{
	public:
		static std::array<size_t, Nodes> analyse(
			const Configuration::DemandAnalysisFlowConfiguration& configuration
		)
		{
			const std::vector<Model::Data::FlowAction>& actions = configuration.getActions();

			// prepare

			std::array<double, Nodes> demand;
			for (auto& demandValue : demand)
				demandValue = 0;

			// get last frame

			size_t timeFrames = 0;
			for (const auto& action : actions)
				timeFrames = std::max(timeFrames, action.flowInstance.destination.timeFrame);
			timeFrames++;

			// prepare memory

			std::vector<std::array<double, Nodes>> memory(timeFrames);

			// iterate actions

			size_t currentAction = 0;
			for (int timeFrame = timeFrames; timeFrame >= 0; timeFrame--)
			{
				for (; currentAction < actions.size() && actions[currentAction].flowInstance.destination.timeFrame == timeFrame; currentAction++)
				{
					const Model::Data::FlowAction& action = actions[currentAction];

					demand[action.flowInstance.destination.node] = std::max(
						0.,
						demand[action.flowInstance.destination.node] - action.actionProbability
					);

					memory[action.flowInstance.source.timeFrame][action.flowInstance.source.node] += action.actionProbability;
				}

				for (size_t node = 0; node < Nodes; node++)
					demand[node] += memory[timeFrame][node];
			}

			// postprocess

			std::array<size_t, Nodes> discretizedDemand;
			for (size_t i = 0; i < Nodes; i++)
				discretizedDemand[i] = (size_t)std::ceil(demand[i]);

			return discretizedDemand;
		}
	};
}