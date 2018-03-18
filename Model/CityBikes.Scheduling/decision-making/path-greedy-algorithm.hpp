#pragma once

#include "../../CityBikes.Data/flow-time/flow-time-prediction.hpp"
#include "fill-greedy-algorithm.hpp"
#include "../decisions/path-decision-applier.hpp"
#include "../decisions/fill-decision-applier.hpp"
#include "../../CityBikes.Data/relocation/relocation-schedule.hpp"

#include <tuple>

namespace CityBikes::Scheduling::DecisionMaking
{
	struct PathDecision
	{
		Data::Relocation::RelocationSchedule operations;
		size_t score;

		PathDecision() :
			score(0)
		{ }
	};

	/// <summary> Makes a greedy decision about the direction relocation unit should take based on greedy fill estimation of the nest step </summary>
	template<size_t Nodes>
	class PathGreedyAlgorithm
	{
	private:
		FillGreedyAlgorithm<Nodes> fillGreedyAlgorithm;

		const Decisions::FillDecisionApplier<Nodes>& fillDecisionApplier;
		const Decisions::PathDecisionApplier<Nodes>& pathDecisionApplier;

	public:
		PathGreedyAlgorithm(
			int minThresholdValue, int maxThresholdValue,
			const Decisions::FillDecisionApplier<Nodes>& fillDecisionApplier,
			const Decisions::PathDecisionApplier<Nodes>& pathDecisionApplier
		) :
			fillGreedyAlgorithm(minThresholdValue, maxThresholdValue),
			fillDecisionApplier(fillDecisionApplier),
			pathDecisionApplier(pathDecisionApplier)
		{ }

		PathDecision makeDecision(
			const Data::FillLevelDistribution::FillLevelDistributionPrediction<Nodes>& networkFillingMatrix,
			Data::Relocation::RelocationUnit relocationUnit,
			size_t length
		) const
		{
			PathDecision decision;

			Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes> alteration(networkFillingMatrix);

			makeFillDecision(alteration, relocationUnit, decision);
			makeDecision(alteration, relocationUnit, decision, length);

			return decision;
		}

	private:
		void makeDecision(
			Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			Data::Relocation::RelocationUnit& relocationUnit,
			PathDecision& decision,
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
			Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			Data::Relocation::RelocationUnit& relocationUnit,
			size_t direction,
			PathDecision& decision
		) const
		{
			pathDecisionApplier.apply(relocationUnit, direction);

			decision.operations.push_back(Data::Relocation::RelocationOperation(direction, 0));

			makeFillDecision(alteration, relocationUnit, decision);
		}

		void makeFillDecision(
			Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			Data::Relocation::RelocationUnit& relocationUnit,
			PathDecision& decision
		) const
		{
			auto fillDecision = fillGreedyAlgorithm.makeDecision(alteration, relocationUnit);
			fillDecisionApplier.apply(relocationUnit, alteration, fillDecision.operation.nodeFillChange);

			decision.operations.push_back(fillDecision.operation);
			decision.score += fillDecision.score;
		}

		size_t score(
			const Data::FillLevelDistribution::FillLevelDistributionPredictionAlteration<Nodes>& alteration,
			Data::Relocation::RelocationUnit relocationUnit,
			size_t direction
		) const
		{
			pathDecisionApplier.apply(relocationUnit, direction);

			return fillGreedyAlgorithm.makeDecision(alteration, relocationUnit).score;
		}
	};
}