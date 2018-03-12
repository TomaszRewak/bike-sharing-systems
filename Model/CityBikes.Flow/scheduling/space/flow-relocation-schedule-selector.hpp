#pragma once

#include "flow-relocation-scheduling-space.hpp"

namespace CityBikes::Flow::Scheduling::Space
{
	template<size_t Nodes>
	class FlowRelocationScheduleSelector
	{
	private:
		FlowRelocationSchedulingSpace<Nodes>& space;
		size_t searchSize;

	public:
		FlowRelocationScheduleSelector(FlowRelocationSchedulingSpace<Nodes>& space, size_t searchSize):
			space(space),
			searchSize(searchSize)
		{ }

		/// <summary> Looks through n random examples to find the best one </summary>
		std::list<Relocation::RelocationOperation> getRoute(const Relocation::RelocationUnit& relocationUnit)
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