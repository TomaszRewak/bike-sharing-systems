#pragma once

#include "../flow-relocation-model.hpp"
#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../decision-making/fill-greedy-algorithm.hpp"
#include "space/flow-relocation-schedule-selector.hpp"

namespace CityBikes::Flow::Scheduling
{
	class FlowRelocationScheduler
	{
	private:
		Space::FlowRelocationScheduleSelector& selector;

	public:
		FlowRelocationScheduler(Space::FlowRelocationScheduleSelector& selector) :
			selector(selector)
		{ }

		/// <summary> Make best decisions for a given (current) time frame. Returns fill change for stations </summary>
		std::vector<std::vector<Relocation::RelocationOperation>> schedule(
			const FlowRelocationModel &relocationModel,
			Model::FlowDistributionModel distributionModel)
		{
			std::vector<std::vector<Relocation::RelocationOperation>> result;

			// Schedule relocations

			for (const auto& relocationUnit : relocationModel.relocationUnits)
			{
				alterDistributionModel(distributionModel, relocationUnit);

				auto operations = selector.getRoute(relocationUnit);

				alterDistributionModel(distributionModel, relocationUnit, operations);

				result.push_back(operations);
			}

			// return result

			return result;
		}

	private:
		void alterDistributionModel(
			Model::FlowDistributionModel& distributionModel,
			Relocation::RelocationUnit relocationUnit,
			std::vector<Relocation::RelocationOperation>& operations)
		{
			for (const auto& operation : operations)
			{
				relocationUnit.schedule(operation);

				alterDistributionModel(distributionModel, relocationUnit);
			}
		}

		void alterDistributionModel(
			Model::FlowDistributionModel& distributionModel,
			const Relocation::RelocationUnit& relocationUnit)
		{
			distributionModel.alter(
				relocationUnit.currentOperation.remainingTime,
				relocationUnit.currentOperation.destination,
				relocationUnit.currentOperation.destinationFillChange);
		}
	};
}