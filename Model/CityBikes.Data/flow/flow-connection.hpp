#pragma once

#include "flow-constraint.hpp"

#include <vector>

namespace CityBikes::Data::Flow
{
	class FlowConnection
	{
	private:
		std::vector<FlowConstraint> constraints;

	public:
		bool reachable() const
		{
			return constraints.size() > 0;
		}

		size_t flowDuration(size_t timeFrame)
		{
			if (timeFrame <= constraints.begin()->timeFrame)
				return constraints.begin()->flowDuration;

			if (timeFrame >= constraints.rbegin()->timeFrame)
				return constraints.rbegin()->flowDuration;

			int constraint = 0;
			while (timeFrame < constraints[constraint].timeFrame)
				constraint++;

			int flowDurationA = constraints[constraint].flowDuration;
			int flowDurationB = constraints[constraint + 1].flowDuration;

			int constraintFrameA = constraints[constraint].timeFrame;
			int constraintFrameB = constraints[constraint + 1].timeFrame;

			return flowDurationA + (flowDurationB - flowDurationA) * (timeFrame - constraintFrameA) / (constraintFrameB - constraintFrameA);
		}

		void addConstraint(FlowConstraint constraint)
		{
			constraints.push_back(constraint);
		}
	};
}