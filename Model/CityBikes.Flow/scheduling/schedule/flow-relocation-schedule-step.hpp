#pragma once

#include <vector>

#include "../../relocation/relocation-unit.hpp"
#include "../../filling/network-filling-matrix-alteration.hpp"

namespace CityBikes::Flow::Scheduling::Schedule
{
	struct FlowRelocationScheduleStep
	{
		size_t destination;
		int destinationFillChange;
		size_t timeFrame;

		FlowRelocationScheduleStep(size_t destination, int destinationFillChange, size_t timeFrame) :
			destination(destination),
			destinationFillChange(destinationFillChange),
			timeFrame(timeFrame)
		{ }

		void applyOn(Relocation::RelocationUnit& relocationUnit)
		{
			relocationUnit.position = destination;
			relocationUnit.state.currentLoad -= destinationFillChange;
			relocationUnit.timeUntilAvailable = timeFrame;
		}

		void applyOn(Filling::NetworkFillingMatrixAlteration& alteration)
		{
			alteration[destination] += destinationFillChange;
		}
	};
}