#pragma once

#include "../../configuration/operation-time-configuration.hpp"
#include "../relocation-unit.hpp"
#include "../../filling/network-filling-matrix-alteration.hpp"

namespace CityBikes::Flow::Relocation::Decision
{
	template<size_t Nodes>
	class FillDecisionApplier
	{
	private:
		const Configuration::OperationTimeConfiguration operationTimeConfiguration;

	public:
		FillDecisionApplier(Configuration::OperationTimeConfiguration operationTimeConfiguration) :
			operationTimeConfiguration(operationTimeConfiguration)
		{ }

		void apply(
			RelocationUnit& relocationUnit,
			int decision
		) const
		{
			relocationUnit.currentLoad -= decision;
			relocationUnit.timeUntilDestination += operationTimeConfiguration.getFillnessChangeTime(decision);
		}

		void apply(
			RelocationUnit& relocationUnit,
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			int decision
		) const
		{
			alteration[relocationUnit.destination] += decision;
			apply(relocationUnit, decision);
		}

		void apply(
			RelocationUnit& relocationUnit,
			Model::Structure::NetworkState<Nodes>& networkState,
			int decision
		) const
		{
			networkState.nodes[relocationUnit.destination].value += decision;
			apply(relocationUnit, decision);
		}

		void apply(
			RelocationUnit& relocationUnit,
			Model::FlowDistributionModel<Nodes>& distributionModel,
			int decision
		) const
		{
			distributionModel.alter(relocationUnit.timeUntilDestination, relocationUnit.destination, decision);
			apply(relocationUnit, decision);
		}

		void check(
			RelocationUnit& relocationUnit,
			int decision
		) const
		{
			if (decision > 0 && relocationUnit.currentLoad < decision
				||
				decision < 0 && relocationUnit.currentLoad + (-decision) > relocationUnit.maxLoad)
				throw "Invalid relocation unit state";
		}

		void check(
			RelocationUnit& relocationUnit,
			Model::Structure::NetworkState<Nodes>& networkState,
			int decision
		) const
		{
			check(relocationUnit, decision);

			if (networkState.nodes[relocationUnit.destination].value + decision < 0)
				throw "Invalid network state";
		}
	};
}