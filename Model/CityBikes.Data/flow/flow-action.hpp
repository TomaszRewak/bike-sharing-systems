#pragma once

#include "flow-instance.hpp"

namespace CityBikes::Data::Flow
{
	struct FlowAction
	{
		FlowInstance flowInstance;

		double actionProbability;

		FlowAction(FlowInstance flowInstance, double actionProbability):
			flowInstance(flowInstance),
			actionProbability(actionProbability)
		{ }
	};
}