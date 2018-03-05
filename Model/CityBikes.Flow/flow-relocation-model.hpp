#pragma once

#include "relocation/relocation-unit.hpp"

#include <vector>

namespace CityBikes::Flow
{
	struct FlowRelocationModel
	{
		std::vector<Relocation::RelocationUnit> relocationUnits;

		FlowRelocationModel(size_t relocationUnits):
			relocationUnits(relocationUnits)
		{ }
	};
}