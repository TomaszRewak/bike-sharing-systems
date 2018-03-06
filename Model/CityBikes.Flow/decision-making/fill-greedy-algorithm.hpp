#pragma once

#include "../filling/network-filling-matrix-alteration.hpp"
#include "../configuration/threshold-configuration.hpp"
#include "../relocation/relocation-limit.hpp"
#include "fill-change-decision.hpp"

namespace CityBikes::Flow::DecisionMaking
{
	class FillGreedyAlgorithm
	{
	private:
		Configuration::ThresholdConfiguration thresholdConfiguration;

		/// <summary> scores a decision based on number of bikes in following time frames that fall outside of threshold - the lower the better </summary>
		int score(Filling::NetworkFillingMatrixAlteration& alteration, size_t timeFrame, size_t node)
		{
			size_t above = alteration.getAboveThreshold(
				timeFrame,
				node,
				thresholdConfiguration.maxValue
			);

			size_t below = alteration.getBelowThreshold(
				timeFrame,
				node,
				thresholdConfiguration.minValue
			);

			return above + below;
		}

	public:
		FillGreedyAlgorithm(Configuration::ThresholdConfiguration thresholdConfiguration) :
			thresholdConfiguration(thresholdConfiguration)
		{ }

		/// <summary> Returns the best decision that's based on greedy scoring </summary>
		FillChangeDecision makeDecision(
			Filling::NetworkFillingMatrixAlteration alteration, 
			size_t timeFrame, size_t node, 
			Relocation::RelocationLimit relocationLimit)
		{
			// Adjust relocation limit

			auto nodeFillingDefinition = alteration.getNodeFillingDefinition(node);

			relocationLimit.minChange = std::max(
				relocationLimit.minChange,
				alteration.getNumber(timeFrame, node)
			);

			// Find best decision

			int bestDecision = relocationLimit.maxChange;
			int bestDecisionScore = std::numeric_limits<int>::max();

			for (int change = relocationLimit.minChange; change < relocationLimit.maxChange; change++)
			{
				alteration.alter(node, change);

				int changeScore = score(alteration, timeFrame, node);
				if (changeScore < bestDecisionScore)
				{
					bestDecisionScore = changeScore;
					bestDecision = change;
				}

				alteration.alter(node, -change);
			}

			// Compare to base score

			int baseScore = score(alteration, timeFrame, node);

			if (baseScore < bestDecisionScore)
				throw "Score should be at least the same";

			return FillChangeDecision(
				bestDecision,
				baseScore - bestDecisionScore
			);
		}
	};
}