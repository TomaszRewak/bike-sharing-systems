#pragma once

#include "relocation/relocation-unit.hpp"

#include <vector>

namespace CityBikes::Flow
{
	struct FlowRelocationModel
	{
		/// <summary> Current time frame of the simulation </summary>
		size_t timeFrame;

		/// <summary> All relocation units in the system (even those that are not currently available due to the ongoing relocation) </summary>
		std::vector<Relocation::RelocationUnit> relocationUnits;

		FlowRelocationModel(size_t startFrame, size_t relocationUnits):
			timeFrame(startFrame),
			relocationUnits(relocationUnits)
		{ }
	};
}