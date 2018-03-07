#pragma once

#include <vector>

#include "flow-relocation-schedule-step.hpp"

namespace CityBikes::Flow::Scheduling::Schedule
{
	struct FlowRelocationSchedule
	{
		/// <summary> The higher the score - the better </summary>
		int score = 0;

		std::vector<FlowRelocationScheduleStep> steps;
	};
}