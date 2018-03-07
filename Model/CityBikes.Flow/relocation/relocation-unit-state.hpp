#pragma once

#include "relocation-limit.hpp"

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnitState
	{
		size_t currentLoad;
		size_t maxLoad;

		RelocationUnitState(size_t maxLoad) :
			maxLoad(maxLoad)
		{ }

		/// <summary> Relocation limit as defined for a station (not for a unit) - how much the state of a station can change </summary>
		RelocationLimit getRelocationLimit()
		{
			return RelocationLimit(
				-(int)(maxLoad - currentLoad),
				currentLoad
			);
		}
	};
}