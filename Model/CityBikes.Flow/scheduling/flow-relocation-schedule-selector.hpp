#pragma once

#include "flow-relocation-scheduling-space.hpp"

namespace CityBikes::Flow::Scheduling
{
	class FlowRelocationScheduleSelector
	{
	private:
		FlowRelocationSchedulingSpace space;
		size_t searchSize;

	public:
		FlowRelocationScheduleSelector(FlowRelocationSchedulingSpace& space, size_t searchSize):
			space(space),
			searchSize(searchSize)
		{ }

		/// <summary> Looks through n random examples to find the best one </summary>
		std::vector<size_t> getRoute(Relocation::RelocationUnit& relocationUnit)
		{
			std::vector<size_t> bestRoute;
			int bestRouteScore = std::numeric_limits<int>::min();

			for (size_t i = 0; i < searchSize; i++)
			{
				auto candidate = space.getNext(relocationUnit);

				if (candidate.score > bestRouteScore)
				{
					bestRouteScore = candidate.score;
					bestRoute = candidate.route;
				}
			}

			return bestRoute;
		}
	};
}