#pragma once

#include "threshold-configuration.hpp"
#include "operation-time-configuration.hpp"
#include "scheduling-space-configuration.hpp"

namespace CityBikes::Flow::Configuration
{
	struct SchedulingConfiguration
	{
		ThresholdConfiguration thresholdConfiguration;
		OperationTimeConfiguration operationTimeConfiguration;
		SchedulingSpaceConfiguration schedulingSpaceConfiguration;

		SchedulingConfiguration(
			ThresholdConfiguration thresholdConfiguration, 
			OperationTimeConfiguration operationTimeConfiguration, 
			SchedulingSpaceConfiguration schedulingSpaceConfiguration
		):
			thresholdConfiguration(thresholdConfiguration),
			operationTimeConfiguration(operationTimeConfiguration),
			schedulingSpaceConfiguration(schedulingSpaceConfiguration)
		{ }
	};
}