#pragma once

#include "relocation-unit-state.hpp"

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		/// <summary> In which time frame the current position will be reached. If not yet reached, relocation unit cannot be used </summary>
		size_t positionReachTime = 0;

		/// <summary> Current state of the relocation unit </summary>
		RelocationUnitState state;

		/// <summary> Next nodes to be visited by this relocation unit </summary>
		std::vector<size_t> route;

		RelocationUnit(size_t maxLoad, size_t currentPosition) :
			state(maxLoad, currentPosition)
		{ }
	};
}