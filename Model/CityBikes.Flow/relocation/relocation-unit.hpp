#pragma once

#include "relocation-operation.hpp"

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		/// <summary> Current load (number of object in the unit) </summary>
		size_t currentLoad;

		/// <summary> Maximal load of the unit </summary>
		size_t maxLoad;

		/// <summary> Currently performed operation (contains information about current position of the unit) </summary>
		RelocationOperation currentOperation;

		RelocationUnit(size_t position, size_t currentLoad, size_t maxLoad) :
			currentOperation(position),
			currentLoad(currentLoad),
			maxLoad(maxLoad)
		{ }

		void schedule(const RelocationOperation& operation)
		{
			currentOperation.destination = operation.destination;
			currentOperation.destinationFillChange = operation.destinationFillChange;
			currentOperation.remainingTime += operation.remainingTime;
		}
	};
}