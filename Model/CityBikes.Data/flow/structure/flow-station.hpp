#pragma once

#include "flow-connection.hpp"

namespace CityBikes::Data::Flow::Structure
{
	class FlowStation
	{
	private:
		std::vector<FlowConnection> connections;

	public:
		FlowStation(size_t stations):
			connections(stations)
		{ }

		void addConstraint(size_t destination, FlowConstraint constraint)
		{
			connections[destination].addConstraint(constraint);
		}

		size_t flowDuration(size_t destination, size_t timeFrame) const
		{
			return connections[destination].flowDuration(timeFrame);
		}
	};
}