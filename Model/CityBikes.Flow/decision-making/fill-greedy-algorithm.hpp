#pragma once

#include "../filling/network-filling-matrix-alteration.hpp"
#include "../configuration/threshold-configuration.hpp"
#include "../configuration/operation-time-configuration.hpp"
#include "decision/fill-decision.hpp"

namespace CityBikes::Flow::DecisionMaking
{
	template<size_t Nodes>
	class FillGreedyAlgorithm
	{
	private:
		const Configuration::ThresholdConfiguration thresholdConfiguration;

	public:
		FillGreedyAlgorithm(Configuration::ThresholdConfiguration thresholdConfiguration) :
			thresholdConfiguration(thresholdConfiguration)
		{ }

		/// <summary> Returns the best decision that's based on greedy scoring </summary>
		Decision::FillDecision makeDecision(
			const Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			const Relocation::RelocationUnit& relocationUnit
		) const
		{
			size_t node = relocationUnit.destination;
			size_t timeFrame = relocationUnit.timeUntilDestination;

			// Compute relocation limit

			int minChange = std::max(
				-alteration.getNumber(timeFrame, node),
				-(int)(relocationUnit.maxLoad - relocationUnit.currentLoad)
			);
			int maxChange = relocationUnit.currentLoad;

			// Find best decision

			Decision::FillDecision bestDecision(relocationUnit.destination);

			for (int change = 1; change <= maxChange; change++)
			{
				size_t decisionScore = score(
					alteration,
					relocationUnit.timeUntilDestination,
					relocationUnit.destination,
					change
				);

				if (decisionScore > bestDecision.score)
				{
					bestDecision.operation.change = change;
					bestDecision.score = decisionScore;
				}
				else if (decisionScore < bestDecision.score)
					break;
			}

			for (int change = -1; change >= minChange; change--)
			{
				size_t decisionScore = score(
					alteration,
					relocationUnit.timeUntilDestination,
					relocationUnit.destination,
					change
				);

				if (decisionScore > bestDecision.score)
				{
					bestDecision.operation.change = change;
					bestDecision.score = decisionScore;
				}
				else if (decisionScore < bestDecision.score)
					break;
			}

			// Return decision

			return bestDecision;
		}

	private:
		/// <summary> scores a decision unit based on its current operation </summary>
		size_t score(
			const Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			size_t timeFrame, size_t node,
			int decision
		) const
		{
			size_t baseError = error(
				alteration,
				timeFrame,
				node,
				0
			);

			size_t fillError = error(
				alteration,
				timeFrame,
				node,
				decision
			);

			if (fillError > baseError)
				return 0;
			else
				return baseError - fillError;
		}

		/// <summary> scores a decision based on number of bikes in following time frames that fall outside of threshold - the lower the better </summary>
		size_t error(
			const Filling::NetworkFillingMatrixAlteration<Nodes>& alteration, 
			size_t timeFrame, size_t node, 
			int decision
		) const
		{
			size_t above = alteration.getAboveThreshold(
				timeFrame,
				node,
				thresholdConfiguration.maxValue - decision
			);

			size_t below = alteration.getBelowThreshold(
				timeFrame,
				node,
				thresholdConfiguration.minValue - decision
			);

			return above + below;
		}
	};
}