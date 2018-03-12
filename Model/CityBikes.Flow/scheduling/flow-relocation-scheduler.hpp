#pragma once

#include "../flow-relocation-model.hpp"
#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../decision-making/fill-greedy-algorithm.hpp"
#include "space/flow-relocation-schedule-selector.hpp"
#include "../configuration/scheduling-configuration.hpp"

namespace CityBikes::Flow::Scheduling
{
	template<size_t Nodes>
	class FlowRelocationScheduler
	{
	private:
		const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowTimeMatrixOffset;
		Configuration::SchedulingConfiguration configuration;

	public:
		FlowRelocationScheduler(
			const Data::FlowTime::FlowTimeMatrixOffset<Nodes>& flowTimeMatrixOffset,
			Configuration::SchedulingConfiguration configuration
		) :
			flowTimeMatrixOffset(flowTimeMatrixOffset),
			configuration(configuration)
		{ }

		/// <summary> Make best decisions for a given (current) time frame. Returns fill change for stations </summary>
		std::vector<std::list<Relocation::RelocationOperation>> schedule(
			FlowRelocationModel relocationModel,
			Model::FlowDistributionModel<Nodes> distributionModel)
		{
			std::vector<std::list<Relocation::RelocationOperation>> result;

			// Schedule relocations

			for (auto& relocationUnit : relocationModel.relocationUnits)
			{
				alterDistributionModel(distributionModel, relocationUnit);

				Filling::NetworkFillingMatrix<Nodes> networkFillingMatrix(
					distributionModel);
				DecisionMaking::FillGreedyAlgorithm<Nodes> fillGreedyAlgorithm(
					configuration.thresholdConfiguration, 
					configuration.operationTimeConfiguration);
				DecisionMaking::DirectionGreedyAlgorithm<Nodes> directionGreedyAlgorithm(
					flowTimeMatrixOffset, 
					fillGreedyAlgorithm);
				Scheduling::Space::FlowRelocationSchedulingSpace<Nodes> schedulingSpace(
					networkFillingMatrix, 
					fillGreedyAlgorithm, 
					directionGreedyAlgorithm, 
					configuration.schedulingSpaceConfiguration.scheduleLength,
					distributionModel.timeFrames.size());
				Scheduling::Space::FlowRelocationScheduleSelector<Nodes> selector(
					schedulingSpace, 
					configuration.schedulingSpaceConfiguration.scheduleSpaceSize);

				auto operations = selector.getRoute(relocationUnit);

				alterDistributionModel(distributionModel, relocationUnit, operations);

				result.push_back(operations);
			}

			// return result

			return result;
		}

	private:
		void alterDistributionModel(
			Model::FlowDistributionModel<Nodes>& distributionModel,
			Relocation::RelocationUnit& relocationUnit,
			std::list<Relocation::RelocationOperation>& operations)
		{
			for (const auto& operation : operations)
			{
				relocationUnit.schedule(operation);

				alterDistributionModel(distributionModel, relocationUnit);
			}
		}

		void alterDistributionModel(
			Model::FlowDistributionModel<Nodes>& distributionModel,
			Relocation::RelocationUnit& relocationUnit)
		{
			distributionModel.alter(
				relocationUnit.currentOperation.remainingTime,
				relocationUnit.currentOperation.destination,
				relocationUnit.currentOperation.destinationFillChange);

			relocationUnit.currentLoad -= relocationUnit.currentOperation.destinationFillChange;
			relocationUnit.currentOperation.destinationFillChange = 0;
		}
	};
}