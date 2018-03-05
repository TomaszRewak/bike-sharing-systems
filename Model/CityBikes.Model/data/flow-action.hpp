#pragma once

#include "flow-instance.hpp"

namespace CityBikes::Model::Data
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