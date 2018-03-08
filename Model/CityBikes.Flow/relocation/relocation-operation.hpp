#pragma once

namespace CityBikes::Flow::Relocation
{
	struct RelocationOperation
	{
		/// <summary> Target node </summary>
		size_t destination;

		/// <summary> Change of the fillness of the station (not the relocation unit!). Aplied after the duration. </summary>
		int destinationFillChange;

		/// <summary> Time needed to perform this decision </summary>
		size_t remainingTime;

		RelocationOperation(size_t destination, int destinationFillChange = 0, size_t duration = 0) :
			destination(destination),
			destinationFillChange(destinationFillChange),
			remainingTime(duration)
		{ }
	};
}