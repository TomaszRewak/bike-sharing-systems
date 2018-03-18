#pragma once

#include "fill-decision-applier.hpp"
#include "path-decision-applier.hpp"
#include "../../CityBikes.Data/relocation/relocation-operation.hpp"

namespace CityBikes::Scheduling::Decisions
{
	template<size_t Nodes>
	class DecisionApplier
	{
	public:
		FillDecisionApplier<Nodes> fillDecisionApplier;
		PathDecisionApplier<Nodes> pathDecisionApplier;

		DecisionApplier(
			double baseOperationTime, double singleOperationTime, 
			const Data::FlowTime::FlowTimePrediction<Nodes>& flowTime, 
			size_t currentTimeFrame
		) :
			fillDecisionApplier(baseOperationTime, singleOperationTime),
			pathDecisionApplier(flowTime, currentTimeFrame)
		{ }

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPrediction<Nodes>& distributionModel,
			Data::Relocation::RelocationOperation relocationOperation
		) const
		{
			pathDecisionApplier.apply(relocationUnit, relocationOperation.destination);
			fillDecisionApplier.apply(relocationUnit, distributionModel, relocationOperation.nodeFillChange);
		}

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPredictionFrame<Nodes>& networkState,
			Data::Relocation::RelocationOperation relocationOperation
		) const
		{
			pathDecisionApplier.apply(relocationUnit, relocationOperation.destination);
			fillDecisionApplier.apply(relocationUnit, networkState, relocationOperation.nodeFillChange);
		}

		void check(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPredictionFrame<Nodes>& networkState,
			Data::Relocation::RelocationOperation relocationOperation
		) const
		{
			fillDecisionApplier.check(relocationUnit, networkState, relocationOperation.nodeFillChange);
		}
	};
}