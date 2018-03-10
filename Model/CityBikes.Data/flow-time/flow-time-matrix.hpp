#pragma once

#include "structure/flow-time-matrix-frame.hpp"

#include <vector>

namespace CityBikes::Data::FlowTime
{
	template<size_t Nodes>
	struct FlowTimeMatrix
	{
		std::vector<Structure::FlowTimeMatrixFrame<Nodes>> timeFrames;

		size_t flowDuration(size_t source, size_t destination, size_t timeFrame) const
		{
			timeFrame = std::min(timeFrame, timeFrames.size() - 1);

			return timeFrames[timeFrame][source][destination];
		}
	};
}