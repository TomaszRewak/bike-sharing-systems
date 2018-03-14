#pragma once

#include "../flow-relocation-model.hpp"
#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../decision-making/fill-greedy-algorithm.hpp"
#include "../decision-making/path-greedy-algorithm.hpp"
#include "../configuration/scheduling-configuration.hpp"
#include "../relocation/decision/decision-applier.hpp"

namespace CityBikes::Flow::Scheduling
{
	template<size_t Nodes>
	class FlowRelocationScheduler
	{
	private:
		Configuration::SchedulingSpaceConfiguration configuration;
		const DecisionMaking::PathGreedyAlgorithm<Nodes>& pathGreedyAlgorithm;
		const Relocation::Decision::DecisionApplier<Nodes>& decisionApplier;

	public:
		FlowRelocationScheduler(
			Configuration::SchedulingSpaceConfiguration configuration,
			const DecisionMaking::PathGreedyAlgorithm<Nodes>& pathGreedyAlgorithm,
			const Relocation::Decision::DecisionApplier<Nodes>& decisionApplier
		) :
			configuration(configuration),
			pathGreedyAlgorithm(pathGreedyAlgorithm),
			decisionApplier(decisionApplier)
		{ }

		/// <summary> Make best decisions for a given (current) time frame. Returns fill change for stations </summary>
		std::vector<std::vector<Relocation::RelocationOperation>> schedule(
			FlowRelocationModel relocationModel,
			Model::FlowDistributionModel<Nodes> distributionModel
		) const
		{
			// prepare result

			std::vector<std::vector<Relocation::RelocationOperation>> result;

			// Schedule relocations

			for (auto& relocationUnit : relocationModel.relocationUnits)
			{
				std::vector<Relocation::RelocationOperation> unitSchedule = scheduleUnit(relocationUnit, distributionModel);

				result.push_back(unitSchedule);
			}

			// return result

			return result;
		}

		std::vector<Relocation::RelocationOperation> scheduleUnit(
			Relocation::RelocationUnit& relocationUnit,
			Model::FlowDistributionModel<Nodes>& distributionModel
		) const
		{
			Filling::NetworkFillingMatrix<Nodes> networkFillingMatrix(distributionModel);

			DecisionMaking::Decision::PathDecision bestDecision;

			for (size_t attempt = 0; attempt < configuration.scheduleSpaceSize; attempt++)
			{
				DecisionMaking::Decision::PathDecision decision = pathGreedyAlgorithm.makeDecision(networkFillingMatrix, relocationUnit, configuration.scheduleLength);

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