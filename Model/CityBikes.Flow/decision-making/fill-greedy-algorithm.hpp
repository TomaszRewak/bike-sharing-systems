#pragma once

#include "../filling/network-filling-matrix-alteration.hpp"
#include "../configuration/threshold-configuration.hpp"
#include "../configuration/operation-time-configuration.hpp"
#include "decision/scored-relocation-operation.hpp"

namespace CityBikes::Flow::DecisionMaking
{
	class FillGreedyAlgorithm
	{
	private:
		Configuration::ThresholdConfiguration thresholdConfiguration;
		Configuration::OperationTimeConfiguration operationTimeConfiguration;

	public:
		FillGreedyAlgorithm(Configuration::ThresholdConfiguration thresholdConfiguration, Configuration::OperationTimeConfiguration operationTimeConfiguration) :
			thresholdConfiguration(thresholdConfiguration),
			operationTimeConfiguration(operationTimeConfiguration)
		{ }

		/// <summary> Returns the best decision that's based on greedy scoring </summary>
		Decision::ScoredRelocationOperation makeDecision(Filling::NetworkFillingMatrixAlteration& alteration, const Relocation::RelocationUnit& relocationUnit) const
		{
			size_t node = relocationUnit.currentOperation.destination;
			size_t timeFrame = relocationUnit.currentOperation.remainingTime;

			// Compute relocation limit

			int minChange = std::max(
				-alteration.getNumber(timeFrame, node),
				-(int)(relocationUnit.maxLoad - relocationUnit.currentLoad)
			);
			int maxChange = relocationUnit.currentLoad;

			// Find best decision

			Decision::ScoredRelocationOperation bestDecision(
				Relocation::RelocationOperation(node, 0, 0),
				0
			);

			for (int change = minChange; change <= maxChange; change++)
			{
				Decision::ScoredRelocationOperation decision = prepareDecision(alteration, relocationUnit, change);

				if (decision.score > bestDecision.score)
					bestDecision = decision;
			}

			// Return decision

			return bestDecision;
		}

	private:
		Decision::ScoredRelocationOperation prepareDecision(Filling::NetworkFillingMatrixAlteration& alteration, Relocation::RelocationUnit relocationUnit, int change) const
		{
			Relocation::RelocationOperation operation(
				relocationUnit.currentOperation.destination,
				change,
				operationTimeConfiguration.getFillnessChangeTime(change)
			);

			relocationUnit.schedule(operation);

			return Decision::ScoredRelocationOperation(
				operation,
				score(alteration, relocationUnit)
			);
		}

		/// <summary> scores a decision unit based on its current operation </summary>
		size_t score(Filling::NetworkFillingMatrixAlteration& alteration, Relocation::RelocationUnit& relocationUnit) const
		{
			size_t baseError = error(
				alteration,
				relocationUnit.currentOperation.remainingTime,
				relocationUnit.currentOperation.destination,
				0
			);

			size_t fillError = error(
				alteration,
				relocationUnit.currentOperation.remainingTime,
				relocationUnit.currentOperation.destination,
				relocationUnit.currentOperation.destinationFillChange
			);

			if (fillError > baseError)
				return 0;
			else
				return baseError - fillError;
		}

		/// <summary> scores a decision based on number of bikes in following time frames that fall outside of threshold - the lower the better </summary>
		size_t error(Filling::NetworkFillingMatrixAlteration& alteration, size_t timeFrame, size_t node, int decision) const
		{
			alteration[node] += decision;

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

			alteration[node] -= decision;

			return above + below;
		}
	};
}