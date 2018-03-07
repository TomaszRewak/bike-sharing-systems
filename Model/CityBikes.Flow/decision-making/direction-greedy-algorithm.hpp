#pragma once

#include "direction-decision.hpp"
#include "../relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/flow/flow-matrix.hpp"
#include "fill-greedy-algorithm.hpp"

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Makes a greedy decision about the direction relocation unit should take based on greedy fill estimation </summary>
	class DirectionGreedyAlgorithm
	{
	private:
		Data::Flow::FlowMatrix& flowMatrix;
		FillGreedyAlgorithm& fillGreedyAlgorithm;

	public:
		DirectionGreedyAlgorithm(Data::Flow::FlowMatrix& flowMatrix, FillGreedyAlgorithm& fillGreedyAlgorithm) :
			flowMatrix(flowMatrix),
			fillGreedyAlgorithm(fillGreedyAlgorithm)
		{ }

		DirectionDecision getDecision(
			Filling::NetworkFillingMatrixAlteration& alteration,
			Relocation::RelocationUnit& relocationUnit)
		{
			// Produce possible decisions

			std::vector<DirectionDecision> possibleDecisions;

			for (size_t node = 0; node < flowMatrix.nodesNumber(); node++)
			{
				size_t transitionTime = flowMatrix.flowDuration(
					relocationUnit.position,
					node,
					relocationUnit.timeUntilAvailable
				);

				size_t score = fillGreedyAlgorithm.makeDecision(
					alteration,
					timeFrame + transitionTime,
					node,
					relocationUnit.getRelocationLimit()
				).score;

				possibleDecisions.push_back(DirectionDecision(
					node,
					transitionTime,
					score
				));
			}

			// Find a sum of all scores and pick a random number in a middle (reoulette selection)

			size_t scoreCount = 0;
			for (auto& possibleDecision : possibleDecisions)
				scoreCount += possibleDecision.score;

			int selectedScore = std::rand() % scoreCount;
			size_t selectedDecision = 0;
			while (selectedScore > possibleDecisions[selectedDecision].score)
			{
				selectedScore -= possibleDecisions[selectedDecision].score;
				selectedDecision++;
			}

			// Return best decision

			return possibleDecisions[selectedDecision];
		}
	};
}