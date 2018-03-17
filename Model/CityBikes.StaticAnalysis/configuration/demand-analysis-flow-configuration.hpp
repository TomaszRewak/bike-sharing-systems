#pragma once

#include "../../CityBikes.Data/flow/flow-action.hpp"

#include <vector>
#include <algorithm>

namespace CityBikes::StaticAnalysis::Configuration
{
	class DemandAnalysisFlowConfiguration
	{
	private:
		std::vector<Data::Flow::FlowAction> actions;

	public:
		DemandAnalysisFlowConfiguration(std::vector<Data::Flow::FlowAction> actions) :
			actions(actions)
		{
			std::sort(
				this->actions.begin(),
				this->actions.end(),
				[](Data::Flow::FlowAction &a, Data::Flow::FlowAction& b) -> bool
			{
				return a.flowInstance.destination.timeFrame > b.flowInstance.destination.timeFrame;
			});
		}

		const std::vector<Data::Flow::FlowAction>& getActions() const
		{
			return actions;
		}
	};
}