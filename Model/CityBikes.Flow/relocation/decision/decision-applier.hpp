#pragma once

#include "fill-decision-applier.hpp"
#include "path-decision-applier.hpp"
#include "../relocation-operation.hpp"

namespace CityBikes::Flow::Relocation::Decision
{
	template<size_t Nodes>
	class DecisionApplier
	{
	private:
		const FillDecisionApplier<Nodes> &fillDecisionApplier;
		const PathDecisionApplier<Nodes> &pathDecisionApplier;

	public:
		DecisionApplier(
			const FillDecisionApplier<Nodes> &fillDecisionApplier,
			const PathDecisionApplier<Nodes> &pathDecisionApplier
		) :
			fillDecisionApplier(fillDecisionApplier),
			pathDecisionApplier(pathDecisionApplier)
		{ }

		void apply(
			Relocation::RelocationUnit& relocationUnit,
			Model::FlowDistributionModel<Nodes>& distributionModel,
			Relocation::RelocationOperation relocationOperation
		) const
		{
			pathDecisionApplier.apply(relocationUnit, relocationOperation.destination);
			fillDecisionApplier.apply(relocationUnit, distributionModel, relocationOperation.change);
		}

		void apply(
			Relocation::RelocationUnit& relocationUnit,
			Model::Structure::NetworkState<Nodes>& networkState,
			Relocation::RelocationOperation relocationOperation
		) const
		{
			pathDecisionApplier.apply(relocationUnit, relocationOperation.destination);
			fillDecisionApplier.apply(relocationUnit, networkState, relocationOperation.change);
		}

		void check(
			Relocation::RelocationUnit& relocationUnit,
			Model::Structure::NetworkState<Nodes>& networkState,
			Relocation::RelocationOperation relocationOperation
		) const
		{
			fillDecisionApplier.check(relocationUnit, networkState, relocationOperation.change);
		}
	};
}