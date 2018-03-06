#pragma once

#include "flow-station.hpp"

#include <vector>

namespace CityBikes::Data::Flow
{
	class FlowMatrix
	{
	private:
		std::vector<FlowStation> stations;

	public:
		FlowMatrix(size_t stations):
			stations(stations, FlowStation(stations))
		{ }

		void addConstraint(size_t source, size_t destination, FlowConstraint constraint)
		{
			stations[source].addConstraint(destination, constraint);
		}

		size_t flowDuration(size_t source, size_t destination, size_t timeFrame)
		{
			return stations[source].flowDuration(destination, timeFrame);
		}

		size_t nodesNumber()
		{
			return stations.size();
		}
	};
}