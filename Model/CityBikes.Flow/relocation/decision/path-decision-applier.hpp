#pragma once

#include "../../../CityBikes.Data/flow-time/flow-time-matrix-offset.hpp"
#include "../relocation-unit.hpp"

namespace CityBikes::Flow::Relocation::Decision
{
	template<size_t Nodes>
	class PathDecisionApplier
	{
	private:
		const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowMatrix;

	public:
		PathDecisionApplier(const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowMatrix) :
			flowMatrix(flowMatrix)
		{ }

		void apply(
			RelocationUnit& relocationUnit,
			size_t destination
		) const
		{
			size_t flowDuration = flowMatrix.flowDuration(
				relocationUnit.destination,
				destination,
				relocationUnit.timeUntilDestination
			);

			relocationUnit.timeUntilDestination += flowDuration;
			relocationUnit.destination = destination;
		}
	};
}