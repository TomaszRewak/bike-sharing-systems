#pragma once

#include "../../CityBikes.Data/relocation/relocation-operation.hpp"
#include "../../CityBikes.Data/relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/fill-level-distribution/fill-level-distribution-prediction-alteration.hpp"

namespace CityBikes::Scheduling::DecisionMaking
{
	struct FillDecision
	{
		Data::Relocation::RelocationOperation operation;
		size_t score;

		FillDecision(size_t node) :
			operation(node, 0),
			score(0)
		{ }
	};

	template<size_t Nodes>
	class FillGreedyAlgorithm
	{
	public:
		const int minThresholdValue;
		const int maxThresholdValue;

		FillGreedyAlgorithm(int minThresholdValue, int maxThresholdValue) :
			minThresholdValue(minThresholdValue),
			maxThresholdValue(maxThresholdValue)
		{ }

		/// <summary> Returns the best decision that's based on greedy scoring </summary>
		FillDecision makeDecision(
			const Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			const Data::Relocation::RelocationUnit& relocationUnit
		) const
		{
			const size_t node = relocationUnit.destination;
			const size_t timeFrame = relocationUnit.timeUntilDestination;

			FillDecision bestDecision(node);

			if (timeFrame >= alteration.timeFrames())
				return bestDecision;

			// Compute relocation limit

			int minChange = std::max(-alteration.getValue(timeFrame, node), -(int)(relocationUnit.maxLoad - relocationUnit.currentLoad));
			int maxChange = relocationUnit.currentLoad;

			// Find best decision

			size_t baseError = error(alteration, timeFrame, node, 0);

			for (int change = minChange; change <= maxChange; change++)
			{
				size_t fillError = error(alteration, timeFrame, node, change);

				if (baseError > fillError && baseError - fillError > bestDecision.score)
				{
					bestDecision.operation.nodeFillChange = change;
					bestDecision.score = baseError - fillError;
				}
			}

			// Return decision

			return bestDecision;
		}

	private:
		/// <summary> scores a decision based on number of bikes in following time frames that fall outside of threshold - the lower the better </summary>
		size_t error(
			const Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			size_t timeFrame, size_t node,
			int change
		) const
		{
			size_t above = alteration.getAboveThreshold(
				timeFrame,
				node,
				maxThresholdValue - change
			);

			size_t below = alteration.getBelowThreshold(
				timeFrame,
				node,
				minThresholdValue - change
			);

			return above + below;
		}
	};
}