#pragma once

#include <map>

#include "flow-distribution-model.hpp"
#include "data/flow-instance.hpp"
#include "data/flow-action.hpp"

namespace CityBikes::Model
{
	class FlowDistributionModelSimulation
	{
	private:
		std::vector<Data::FlowAction> actions;

	public:

		FlowDistributionModelSimulation(std::vector<Data::FlowAction> actions) :
			actions(actions)
		{
			std::sort(
				this->actions.begin(),
				this->actions.end(),
				[](Data::FlowAction &a, Data::FlowAction& b) -> bool
			{
				return a.flowInstance.source.timeFrame < b.flowInstance.source.timeFrame;
			});
		}

		FlowDistributionModel simulate(Structure::NetworkState initialState, size_t startFrame, size_t endFrame, bool forceFlow)
		{
			size_t timeFrames = endFrame - startFrame;

			FlowDistributionModel model(timeFrames, initialState.nodes.size());
			model.startFrame = startFrame;
			model.timeFrames[0] = initialState;

			size_t currentAction = 0;

			for (size_t modelFrame = 0; modelFrame < timeFrames; modelFrame++)
			{
				if (modelFrame > 0)
					for (size_t node = 0; node < initialState.nodes.size(); node++)
						model.timeFrames[modelFrame].nodes[node].value += model.timeFrames[modelFrame - 1].nodes[node].value;

				for (; 
					currentAction < actions.size() &&
					actions[currentAction].flowInstance.source.timeFrame <= startFrame + modelFrame; 
					currentAction++)
				{
					Data::FlowAction& action = actions[currentAction];

					double getChance = 0;

					if (action.flowInstance.source.timeFrame < startFrame)
						getChance = action.actionProbability;
					else if (action.flowInstance.source.timeFrame < endFrame)
						getChance = model.timeFrames[action.flowInstance.source.timeFrame - startFrame].nodes[action.flowInstance.source.node].get(action.actionProbability, forceFlow);

					if (action.flowInstance.destination.timeFrame >= startFrame && action.flowInstance.destination.timeFrame < endFrame)
						model.timeFrames[action.flowInstance.destination.timeFrame - startFrame].nodes[action.flowInstance.destination.node].add(getChance);
				}
			}

			return model;
		}
	};
}