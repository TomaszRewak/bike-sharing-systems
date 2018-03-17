#pragma once

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		/// <summary> Current load (number of object in the unit) </summary>
		size_t currentLoad;

		/// <summary> Maximal load of the unit </summary>
		size_t maxLoad;

		/// <summary> The next destination of the unit or current position if timeUntilDestination == 0 </summary>
		size_t destination;

		/// <summary> When is destination going to be reached </summary>
		float timeUntilDestination;

		RelocationUnit(size_t position, size_t currentLoad, size_t maxLoad) :
			destination(position),
			currentLoad(currentLoad),
			maxLoad(maxLoad),
			timeUntilDestination(0)
		{ }
	};
}