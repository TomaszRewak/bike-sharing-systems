#pragma once

#include "../../CityBikes.Data/relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/flow-time/flow-time-prediction.hpp"

namespace CityBikes::Scheduling::Decisions
{
	template<size_t Nodes>
	class PathDecisionApplier
	{
	private:
		const Data::FlowTime::FlowTimePrediction<Nodes>& flowTime;
		const size_t currentTimeFrame;

	public:
		PathDecisionApplier(const Data::FlowTime::FlowTimePrediction<Nodes>& flowTime, size_t currentTimeFrame) :
			flowTime(flowTime),
			currentTimeFrame(currentTimeFrame)
		{ }

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			size_t destination
		) const
		{
			double flowDuration = flowTime.flowDuration(
				relocationUnit.destination,
				destination,
				relocationUnit.timeUntilDestination + currentTimeFrame
			);

			relocationUnit.timeUntilDestination += flowDuration;
			relocationUnit.destination = destination;
		}
	};
}