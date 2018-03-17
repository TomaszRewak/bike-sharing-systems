#pragma once

#include "../../CityBikes.Data/flow/flow-action.hpp"

#include <vector>
#include <algorithm>
#include <functional>

namespace CityBikes::DataProcessing::Flow
{
	class Sorting
	{
	public:
		static void sortActionsAscendingByStartTime(std::vector<Data::Flow::FlowAction>& actions)
		{
			std::sort(
				actions.begin(),
				actions.end(),
				[](Data::Flow::FlowAction &a, Data::Flow::FlowAction& b) -> bool
			{
				return a.flowInstance.source.timeFrame < b.flowInstance.source.timeFrame;
			});
		}

		static void sortInstancesDescendingByStartTime(std::vector<Data::Flow::FlowInstance>& instances)
		{
			std::sort(
				instances.begin(),
				instances.end(),
				[](const Data::Flow::FlowInstance &a, const Data::Flow::FlowInstance& b) -> bool
			{
				return a.source.timeFrame > b.source.timeFrame;
			});
		}

		static void sortInstancesDescendingByEndTime(std::vector<Data::Flow::FlowInstance>& instances)
		{
			std::sort(
				instances.begin(),
				instances.end(),
				[](const Data::Flow::FlowInstance &a, const Data::Flow::FlowInstance& b) -> bool
			{
				return a.destination.timeFrame > b.destination.timeFrame;
			});
		}
	};
}