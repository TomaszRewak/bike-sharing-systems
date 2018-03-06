#pragma once

#include <vector>

namespace CityBikes::Flow::Scheduling
{
	struct FlowRelocationSchedule
	{
		/// <summary> The higher the score - the better </summary>
		int score = 0;

		std::vector<size_t> route;
	};
}