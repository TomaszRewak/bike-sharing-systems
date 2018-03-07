#pragma once

#include "relocation-unit-state.hpp"

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		/// <summary> The position given relocation unit is currenly on or is heading to </summary>
		size_t position;

		/// <summary> After how many time frames the position will be reached </summary>
		size_t timeUntilAvailable = 0;

		/// <summary> Current state of the relocation unit </summary>
		RelocationUnitState state;

		RelocationUnit(size_t maxLoad, size_t position) :
			state(maxLoad),
			position(position)
		{ }
	};
}