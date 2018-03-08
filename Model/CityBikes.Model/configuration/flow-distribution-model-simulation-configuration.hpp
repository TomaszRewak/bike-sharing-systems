#pragma once

#include "../data/flow-action.hpp"

#include <vector>
#include <algorithm>

namespace CityBikes::Model::Configuration
{
	class FlowDistributionModelSimulationConfiguration
	{
	private:
		std::vector<Data::FlowAction> actions;

	public:
		FlowDistributionModelSimulationConfiguration(std::vector<Data::FlowAction> actions) :
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

		const std::vector<Data::FlowAction>& getActions() const
		{
			return actions;
		}
	};
}