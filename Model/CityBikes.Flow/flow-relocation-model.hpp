#pragma once

#include "relocation/relocation-unit.hpp"

#include <vector>

namespace CityBikes::Flow
{
	struct FlowRelocationModel
	{
		size_t timeFrame;

		std::vector<Relocation::RelocationUnit> relocationUnits;

		FlowRelocationModel(size_t startFrame, size_t relocationUnits):
			timeFrame(startFrame),
			relocationUnits(relocationUnits)
		{ }
	};
}