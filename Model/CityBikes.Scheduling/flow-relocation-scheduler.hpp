#pragma once

#include "decision-making/path-greedy-algorithm.hpp"
#include "decisions/decision-applier.hpp"
#include "../CityBikes.Data/relocation/relocation-team.hpp"

namespace CityBikes::Scheduling
{
	template<size_t Nodes>
	class FlowRelocationScheduler
	{
	private:
		const Decisions::DecisionApplier<Nodes>& decisionApplier;

		DecisionMaking::PathGreedyAlgorithm<Nodes> pathGreedyAlgorithm;

		const size_t scheduleLength;
		const size_t scheduleExamples;

	public:
		FlowRelocationScheduler(
			const Decisions::DecisionApplier<Nodes>& decisionApplier,
			int minThresholdValue, int maxThresholdValue,
			size_t scheduleLength, size_t scheduleExamples
		) :
			decisionApplier(decisionApplier),
			pathGreedyAlgorithm(minThresholdValue, maxThresholdValue, decisionApplier.fillDecisionApplier, decisionApplier.pathDecisionApplier),
			scheduleLength(scheduleLength),
			scheduleExamples(scheduleExamples)
		{ }

		/// <summary> Make best decisions for a given (current) time frame. Returns fill change for stations </summary>
		std::vector<Data::Relocation::RelocationSchedule> schedule(
			Data::Relocation::RelocationTeam relocationModel,
			Data::FillLevel::FillLevelPrediction<Nodes> distributionModel
		) const
		{
			// prepare result

			std::vector<Data::Relocation::RelocationSchedule> result;

			// Schedule relocations

			for (auto& relocationUnit : relocationModel)
			{
				Data::Relocation::RelocationSchedule unitSchedule = scheduleUnit(relocationUnit, distributionModel);

				result.push_back(unitSchedule);
			}

			// return result

			return result;
		}

		Data::Relocation::RelocationSchedule scheduleUnit(
			Data::Relocation::RelocationUnit& relocationUnit,
			Data::FillLevel::FillLevelPrediction<Nodes>& distributionModel
		) const
		{
			Data::FillLevelDistribution::FillLevelDistributionPrediction<Nodes> networkFillingMatrix(distributionModel);

			DecisionMaking::PathDecision bestDecision;

			for (size_t attempt = 0; attempt < scheduleExamples; attempt++)
			{
				DecisionMaking::PathDecision decision = 
					pathGreedyAlgorithm.makeDecision(networkFillingMatrix, relocationUnit, scheduleLength);

				if (decision.score > bestDecision.score)
					bestDecision = decision;
			}

			for (auto& operation : bestDecision.operations)
			{
				decisionApplier.apply(relocationUnit, distributionModel, operation);
			}

			return bestDecision.operations;
		}
	};
}