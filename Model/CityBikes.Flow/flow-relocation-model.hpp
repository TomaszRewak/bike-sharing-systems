#pragma once

#include "relocation/relocation-unit.hpp"

#include <vector>

namespace CityBikes::Flow
{
	struct FlowRelocationModel
	{
		/// <summary> All relocation units in the system (even those that are not currently available due to the ongoing relocation) </summary>
		std::vector<Relocation::RelocationUnit> relocationUnits;

		FlowRelocationModel(size_t relocationUnits, size_t baseNode, size_t initialLoad, size_t maxLoad):
			relocationUnits(relocationUnits, Relocation::RelocationUnit(baseNode, initialLoad, maxLoad))
		{ }
	};
}