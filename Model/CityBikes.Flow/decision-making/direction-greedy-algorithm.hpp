#pragma once

#include "decision/scored-relocation-operation.hpp"
#include "../relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/flow-time/flow-time-matrix-offset.hpp"
#include "fill-greedy-algorithm.hpp"

#include <tuple>

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Makes a greedy decision about the direction relocation unit should take based on greedy fill estimation of the nest step </summary>
	template<size_t Nodes>
	class DirectionGreedyAlgorithm
	{
	private:
		const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowMatrix;
		const FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm;

	public:
		DirectionGreedyAlgorithm(const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowMatrix, const FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm) :
			flowMatrix(flowMatrix),
			fillGreedyAlgorithm(fillGreedyAlgorithm)
		{ }

		std::pair<Decision::ScoredRelocationOperation, Decision::ScoredRelocationOperation> getDecision(
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit& relocationUnit)
		{
			// Produce all possible decisions - relocation to all nodes

			std::vector<std::pair<Decision::ScoredRelocationOperation, Decision::ScoredRelocationOperation>> possibleDecisions;

			for (size_t node = 0; node < Nodes; node++)
			{
				auto operations = prepareOperations(alteration, relocationUnit, node);

				possibleDecisions.push_back(operations);
			}

			// Find a sum of all scores and pick a random number in a middle (reoulette selection)

			size_t scoreCount = 0;
			for (auto& possibleDecision : possibleDecisions)
				scoreCount += possibleDecision.second.score;

			if (scoreCount == 0)
				return prepareOperations(alteration, relocationUnit, relocationUnit.currentOperation.destination, 1);

			size_t selectedDecision = 0;
			size_t selectedScore = std::rand() % scoreCount;

			while (selectedScore > possibleDecisions[selectedDecision].second.score)
			{
				selectedScore -= possibleDecisions[selectedDecision].second.score;
				selectedDecision++;
			}

			// Return best decision

			return possibleDecisions[selectedDecision];
		}

	private:
		std::pair<Decision::ScoredRelocationOperation, Decision::ScoredRelocationOperation> prepareOperations(
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit relocationUnit,
			size_t targetNode,
			size_t minTransitionTime = 0)
		{
			size_t transitionTime = flowMatrix.flowDuration(
				relocationUnit.currentOperation.destination,
				targetNode,
				relocationUnit.currentOperation.remainingTime
			);

			transitionTime = std::max(minTransitionTime, transitionTime);

			auto moveOperation = Decision::ScoredRelocationOperation(
				Relocation::RelocationOperation(targetNode, 0, transitionTime),
				0
			);

			relocationUnit.schedule(moveOperation.operation);

			auto fillOperation = fillGreedyAlgorithm.makeDecision(
				alteration,
				relocationUnit
			);

			return std::make_pair(moveOperation, fillOperation);
		}
	};
}