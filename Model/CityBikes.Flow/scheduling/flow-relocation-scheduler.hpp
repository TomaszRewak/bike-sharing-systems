#pragma once

#include "../flow-relocation-model.hpp"
#include "../relocation/relocation-task.hpp"
#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../decision-making/fill-greedy-algorithm.hpp"

namespace CityBikes::Flow::Scheduling
{
	class FlowRelocationScheduler
	{
	private:
		DecisionMaking::FillGreedyAlgorithm fillGreedyAlgorithm;

	public:
		/// <summary> Make best decisions for a given (current) time frame. Returns fill change for stations </summary>
		std::vector<int> schedule(
			FlowRelocationModel &relocationModel,
			Model::FlowDistributionModel distributionModel)
		{
			std::vector<int> fillChanges(distributionModel.nodes, 0);

			// Check if any relocation unit is awaiting

			bool hasAwaitingUnits = false;
			for (auto& relocationUnit : relocationModel.relocationUnits)
				if (relocationUnit.timeUntilAvailable == 0)
					hasAwaitingUnits = true;

			if (!hasAwaitingUnits)
				return fillChanges;

			// Schedule relocations

			for (auto& relocationUnit : relocationModel.relocationUnits)
			{
			}

			// Schedule new relocations
		}
	};
}