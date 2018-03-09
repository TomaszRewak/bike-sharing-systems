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

		size_t currentAction = 0;

		Structure::NetworkState<Nodes> currentState;
		FlowDistributionModel<Nodes> model;
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

		FlowDistributionModel<Nodes> getModel() const
		{
			return model;
		}

		Structure::NetworkState<Nodes>& getCurrentState()
		{
			return currentState;
		}

		void step()
		{
			size_t timeFrame = model.timeFrames.size();

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

			model.timeFrames.push_back(currentState);
		}

		void runTo(size_t timeFrame)
		{
			while (model.timeFrames.size() < timeFrame)
				step();
		}
	};
}