#pragma once

#include "decision/path-decision.hpp"
#include "../relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/flow-time/flow-time-prediction.hpp"
#include "fill-greedy-algorithm.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../configuration/operation-time-configuration.hpp"
#include "../relocation/decision/fill-decision-applier.hpp"
#include "../relocation/decision/path-decision-applier.hpp"

#include <tuple>

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Makes a greedy decision about the direction relocation unit should take based on greedy fill estimation of the nest step </summary>
	template<size_t Nodes>
	class PathGreedyAlgorithm
	{
	private:
		const FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm;
		const Relocation::Decision::FillDecisionApplier<Nodes>& fillDecisionApplier;
		const Relocation::Decision::PathDecisionApplier<Nodes>& pathDecisionApplier;

	public:
		PathGreedyAlgorithm(
			const FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm,
			const Relocation::Decision::FillDecisionApplier<Nodes>& fillDecisionApplier,
			const Relocation::Decision::PathDecisionApplier<Nodes>& pathDecisionApplier
		) :
			fillGreedyAlgorithm(fillGreedyAlgorithm),
			fillDecisionApplier(fillDecisionApplier),
			pathDecisionApplier(pathDecisionApplier)
		{ }

		Decision::PathDecision makeDecision(
			const Filling::NetworkFillingMatrix<Nodes>& networkFillingMatrix,
			Relocation::RelocationUnit relocationUnit,
			size_t length
		) const
		{
			Decision::PathDecision decision;

			Filling::NetworkFillingMatrixAlteration<Nodes> alteration(networkFillingMatrix);

			makeFillDecision(alteration, relocationUnit, decision);
			makeDecision(alteration, relocationUnit, decision, length);

			return decision;
		}

	private:
		void makeDecision(
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit& relocationUnit,
			Decision::PathDecision& decision,
			size_t length
		) const
		{
			if (length == 0)
				return;

			// Produce all possible decisions - relocation to all nodes

			std::array<size_t, Nodes> possibleDecisionScores;

			for (size_t destination = 0; destination < Nodes; destination++)
			{
				possibleDecisionScores[destination] = score(alteration, relocationUnit, destination);
			}

			// Find a sum of all scores and pick a random number in a middle (reoulette selection)

			size_t scoreCount = 0;
			for (auto& possibleDecisionScore : possibleDecisionScores)
				scoreCount += possibleDecisionScore;

			if (scoreCount == 0)
				return;

			size_t selectedDirection = 0;
			size_t selectedScore = std::rand() % scoreCount;

			while (selectedScore > possibleDecisionScores[selectedDirection])
			{
				selectedScore -= possibleDecisionScores[selectedDirection];
				selectedDirection++;
			}

			// Apply decision

			makePathDecision(alteration, relocationUnit, selectedDirection, decision);

			// Next step

			makeDecision(alteration, relocationUnit, decision, length - 1);
		}

		void makePathDecision(
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit& relocationUnit,
			size_t direction,
			Decision::PathDecision& decision
		) const
		{
			pathDecisionApplier.apply(relocationUnit, direction);

			decision.operations.push_back(Relocation::RelocationOperation(direction, 0));

			makeFillDecision(alteration, relocationUnit, decision);
		}

		void makeFillDecision(
			Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit& relocationUnit,
			Decision::PathDecision& decision
		) const
		{
			auto fillDecision = fillGreedyAlgorithm.makeDecision(alteration, relocationUnit);
			fillDecisionApplier.apply(relocationUnit, alteration, fillDecision.operation.change);

			decision.operations.push_back(fillDecision.operation);
			decision.score += fillDecision.score;
		}

		size_t score(
			const Filling::NetworkFillingMatrixAlteration<Nodes>& alteration,
			Relocation::RelocationUnit relocationUnit,
			size_t direction
		) const
		{
			pathDecisionApplier.apply(relocationUnit, direction);

			return fillGreedyAlgorithm.makeDecision(alteration, relocationUnit).score;
		}
	};
}