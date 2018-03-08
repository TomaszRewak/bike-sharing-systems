#pragma once

#include "flow-relocation-scheduling-space.hpp"

namespace CityBikes::Flow::Scheduling::Space
{
	class FlowRelocationScheduleSelector
	{
	private:
		FlowRelocationSchedulingSpace& space;
		size_t searchSize;

	public:
		FlowRelocationScheduleSelector(FlowRelocationSchedulingSpace& space, size_t searchSize):
			space(space),
			searchSize(searchSize)
		{ }

		/// <summary> Looks through n random examples to find the best one </summary>
		std::vector<Relocation::RelocationOperation> getRoute(const Relocation::RelocationUnit& relocationUnit)
		{
			Schedule::FlowRelocationSchedule bestSchedule;

			for (size_t i = 0; i < searchSize; i++)
			{
				auto candidate = space.getNext(relocationUnit);

				if (candidate.score > bestSchedule.score)
					bestSchedule = candidate;
			}

			return bestSchedule.operations;
		}
	};
}