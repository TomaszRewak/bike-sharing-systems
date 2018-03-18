#pragma once

#include "../../CityBikes.Data/flow/flow-instance.hpp"

#include <vector>

namespace CityBikes::DataProcessing::Flow
{
	class Casting
	{
	public:
		static std::vector<Data::Flow::FlowTarget> getOngoingFlow(const std::vector<Data::Flow::FlowInstance>& instances)
		{
			std::vector<Data::Flow::FlowTarget> ongoingFlow;

			for (const Data::Flow::FlowInstance& ongoingInstance : instances)
				ongoingFlow.push_back(ongoingInstance.source);

			return ongoingFlow;
		}
	};
}