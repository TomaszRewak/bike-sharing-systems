#pragma once

#include "../../CityBikes.Data/relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/fill-level-distribution/fill-level-distribution-prediction-alteration.hpp"

namespace CityBikes::Scheduling::Decisions
{
	template<size_t Nodes>
	class FillDecisionApplier
	{
	private:
		const double baseOperationTime;
		const double singleOperationTime;

		double getFillnessChangeTime(int fillChange) const
		{
			if (fillChange == 0)
				return 0;

			return baseOperationTime + std::abs(fillChange) * singleOperationTime;
		}

	public:
		FillDecisionApplier(double baseOperationTime, double singleOperationTime) :
			baseOperationTime(baseOperationTime),
			singleOperationTime(singleOperationTime)
		{ }

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			int decision
		) const
		{
			relocationUnit.currentLoad -= decision;
			relocationUnit.timeUntilDestination += getFillnessChangeTime(decision);
		}

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			int decision
		) const
		{
			alteration[relocationUnit.destination] += decision;
			apply(relocationUnit, decision);
		}

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPredictionFrame<Nodes>& networkState,
			int decision
		) const
		{
			networkState[relocationUnit.destination] += decision;
			apply(relocationUnit, decision);
		}

		void apply(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPrediction<Nodes>& distributionModel,
			int decision
		) const
		{
			for (size_t timeFrame = relocationUnit.timeUntilDestination; timeFrame < distributionModel.size(); timeFrame++)
				distributionModel[timeFrame][relocationUnit.destination] += decision;
		}

		void check(
			Data::Relocation::RelocationUnit& relocationUnit,
			int decision
		) const
		{
			if (decision > 0 && relocationUnit.currentLoad < decision
				||
				decision < 0 && relocationUnit.currentLoad + (-decision) > relocationUnit.maxLoad)
				throw "Invalid relocation unit state";
		}

		void check(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPredictionFrame<Nodes>& networkState,
			int decision
		) const
		{
			check(relocationUnit, decision);

			if (networkState[relocationUnit.destination] + decision < 0)
				throw "Invalid network state";
		}

		void fix(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPredictionFrame<Nodes>& networkState,
			int& decision
		) const
		{
			decision = std::max(decision, -(int)networkState[relocationUnit.destination]);
			decision = std::min(decision, (int)relocationUnit.currentLoad);
			decision = std::max(decision, (int)relocationUnit.currentLoad - (int)relocationUnit.maxLoad);
		}
	};
}