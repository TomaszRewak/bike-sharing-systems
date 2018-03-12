#pragma once

namespace CityBikes::Flow::Configuration
{
	struct SchedulingSpaceConfiguration
	{
		size_t scheduleLength;
		size_t scheduleSpaceSize;

		SchedulingSpaceConfiguration(size_t scheduleLength, size_t scheduleSpaceSize):
			scheduleLength(scheduleLength),
			scheduleSpaceSize(scheduleSpaceSize)
		{ }
	};
}