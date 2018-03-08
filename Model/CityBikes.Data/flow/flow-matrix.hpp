#pragma once

#include "structure/flow-station.hpp"

#include <vector>

namespace CityBikes::Data::Flow
{
	class FlowMatrix // TODO: change to full matrix instaed of collection of constraints
	{
	private:
		std::vector<Structure::FlowStation> stations;

	public:
		FlowMatrix(size_t stations):
			stations(stations, Structure::FlowStation(stations))
		{ }

		void addConstraint(size_t source, size_t destination, Structure::FlowConstraint constraint)
		{
			stations[source].addConstraint(destination, constraint);
		}

		size_t flowDuration(size_t source, size_t destination, size_t timeFrame) const
		{
			return stations[source].flowDuration(destination, timeFrame);
		}

		size_t nodesNumber()
		{
			return stations.size();
		}
	};
}