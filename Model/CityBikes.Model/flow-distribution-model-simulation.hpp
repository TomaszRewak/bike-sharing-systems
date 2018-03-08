#pragma once

#include <map>

#include "flow-distribution-model.hpp"
#include "data/flow-instance.hpp"
#include "data/flow-action.hpp"
#include "configuration/flow-distribution-model-simulation-configuration.hpp"
#include "memory/flow-distribution-model-memory.hpp"

namespace CityBikes::Model
{
	class FlowDistributionModelSimulation
	{
	private:
		const Configuration::FlowDistributionModelSimulationConfiguration& configuration;

		const size_t timeFrames;

		size_t timeFrame = 0;
		size_t currentAction = 0;

		FlowDistributionModel model;
		Memory::FlowDistributionModelMemory memory;

		const bool forceFlow;

	public:
		FlowDistributionModelSimulation(
			const Configuration::FlowDistributionModelSimulationConfiguration& configuration,
			Structure::NetworkState initialState,
			size_t timeFrames,
			bool forceFlow
		) :
			configuration(configuration),
			model(initialState),
			timeFrames(timeFrames),
			forceFlow(forceFlow),
			memory(timeFrames, initialState.nodes.size())
		{ }

		FlowDistributionModel getModel() const
		{
			return model;
		}

		Structure::NetworkState& getCurrentState()
		{
			return model.timeFrames[timeFrame];
		}

		void step()
		{
			if (timeFrame >= timeFrames)
				throw "Time frame out of bounds";

			// apply pending returns

			for (size_t node = 0; node < model.nodes; node++)
				model.timeFrames[timeFrame].nodes[node].value += memory.timeFrames[timeFrame][node];

			// perform actions

			const auto& actions = configuration.getActions();

			for (;
				currentAction < actions.size() &&
				actions[currentAction].flowInstance.source.timeFrame <= timeFrame;
				currentAction++)
			{
				const auto& action = actions[currentAction];

				double getChance = model
					.timeFrames[action.flowInstance.source.timeFrame]
					.nodes[action.flowInstance.source.node]
					.get(action.actionProbability, forceFlow);

				if (action.flowInstance.destination.timeFrame < timeFrames)
					memory.timeFrames[action.flowInstance.destination.timeFrame][action.flowInstance.destination.node] += getChance;
			}

			// copy last state

			auto currentState = getCurrentState();
			model.timeFrames.push_back(currentState);

			timeFrame++;
		}

		FlowDistributionModel runToEnd()
		{
			while (timeFrame < timeFrames)
				step();
		}
	};
}