#pragma once

#include <vector>

namespace CityBikes::Flow::Scheduling::Schedule
{
	struct FlowRelocationScheduleStep
	{
		size_t destination;
		int destinationFillChange;
		size_t timeFrame;

		FlowRelocationScheduleStep(size_t destination, int destinationFillChange, size_t timeFrame) :
			destination(destination),
			destinationFillChange(destinationFillChange),
			timeFrame(timeFrame)
		{ }
	};
}