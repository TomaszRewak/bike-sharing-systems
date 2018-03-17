#pragma once

#include "../relocation-unit.hpp"

namespace CityBikes::Flow::Relocation::Decision
{
	template<size_t Nodes>
	class PathDecisionApplier
	{
	private:
		const Data::FlowTime::FlowTimePredictionOffset<Nodes>& flowMatrix;

	public:
		PathDecisionApplier(const Data::FlowTime::FlowTimePredictionOffset<Nodes>& flowMatrix) :
			flowMatrix(flowMatrix)
		{ }

		void apply(
			RelocationUnit& relocationUnit,
			size_t destination
		) const
		{
			float flowDuration = flowMatrix.flowDuration(
				relocationUnit.destination,
				destination,
				relocationUnit.timeUntilDestination
			);

			relocationUnit.timeUntilDestination += flowDuration;
			relocationUnit.destination = destination;
		}
	};
}