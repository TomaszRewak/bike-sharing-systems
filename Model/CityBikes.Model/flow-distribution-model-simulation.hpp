#pragma once

#include <map>

#include "flow-distribution-model.hpp"
#include "data/flow-instance.hpp"
#include "data/flow-action.hpp"
#include "configuration/flow-distribution-model-simulation-configuration.hpp"
#include "memory/flow-distribution-model-memory.hpp"

namespace CityBikes::Model
{
	template<size_t Nodes>
	class FlowDistributionModelSimulation
	{
	private:
		const Configuration::FlowDistributionModelSimulationConfiguration& configuration;

		size_t timeFrame = 0;
		size_t currentAction = 0;

		Structure::NetworkState<Nodes> currentState;
		Memory::FlowDistributionModelMemory<Nodes> memory;

		const bool forceFlow;

	public:
		FlowDistributionModelSimulation(
			const Configuration::FlowDistributionModelSimulationConfiguration& configuration,
			Structure::NetworkState<Nodes> initialState,
			bool forceFlow
		) :
			configuration(configuration),
			currentState(initialState),
			forceFlow(forceFlow)
		{ }

		Structure::NetworkState<Nodes>& getCurrentState()
		{
			return currentState;
		}

		void step()
		{
			// perform actions

			const auto& actions = configuration.getActions();

			for (;
				currentAction < actions.size() &&
				actions[currentAction].flowInstance.source.timeFrame <= timeFrame;
				currentAction++)
			{
				const auto& action = actions[currentAction];

				double getChance = currentState
					.nodes[action.flowInstance.source.node]
					.get(action.actionProbability, forceFlow);

				memory.add(
					action.flowInstance.destination.timeFrame, 
					action.flowInstance.destination.node,
					getChance);
			}

			// apply pending returns

			auto pendingReturns = memory.pop(timeFrame);

			for (size_t node = 0; node < Nodes; node++)
				currentState.nodes[node].value += pendingReturns[node];

			// update model

			timeFrame++;
		}

		FlowDistributionModel<Nodes> run(size_t timeFrames)
		{
			FlowDistributionModel<Nodes> model;

			for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
			{
				model.timeFrames.push_back(currentState);
				step();
			}

			return model;
		}
	};
}