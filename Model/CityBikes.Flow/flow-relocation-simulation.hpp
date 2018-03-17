#pragma once

#include "flow-relocation-model.hpp"
#include "../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../CityBikes.Model/flow-prediction-simulation.hpp"
#include "scheduling/flow-relocation-scheduler.hpp"
#include "configuration/simulation-configuration.hpp"

namespace CityBikes::Flow
{
	template<size_t Nodes>
	class FlowRelocationSimulation
	{
	private:
		const Configuration::SimulationConfiguration& configuration;

		const Data::FlowTime::FlowTimePrediction<Nodes>& flowTimeMatrix;

		Model::FlowDistributionModelSimulation<Nodes> realFlowSimulation;
		Model::FlowDistributionModelSimulation<Nodes> predictedFlowSimulation;

		size_t timeFrame = 0;
		FlowRelocationModel relocationModel;

	public:
		FlowRelocationSimulation(
			Model::Structure::NetworkState<Nodes> initialState,
			FlowRelocationModel relocationModel,
			const Configuration::SimulationConfiguration& configuration,
			const Data::FlowTime::FlowTimePrediction<Nodes>& flowTimeMatrix
		) :
			configuration(configuration),
			flowTimeMatrix(flowTimeMatrix),
			realFlowSimulation(configuration.realFlowSimulationConfiguration, initialState, false),
			predictedFlowSimulation(configuration.predictedFlowSimulationConfiguration, initialState, true),
			relocationModel(relocationModel)
		{ }

		Model::Structure::NetworkState<Nodes>& getCurrentState()
		{
			return realFlowSimulation.getCurrentState();
		}

		FlowRelocationModel& getCurrentModel()
		{
			return relocationModel;
		}

		void step()
		{
			Model::FlowDistributionModelSimulation<Nodes> predictionWindowSimulation = predictedFlowSimulation;
			Model::FlowDistributionModel<Nodes> predictedWindow = predictionWindowSimulation.run(configuration.relocationSchedulingConfiguration.predictionWindow);

			Data::FlowTime::FlowTimePredictionOffset<Nodes> flowTimeMatrixOffset(
				flowTimeMatrix,
				timeFrame);
			Relocation::Decision::FillDecisionApplier<Nodes> fillDecisionApplier(
				configuration.schedulingConfiguration.operationTimeConfiguration);
			Relocation::Decision::PathDecisionApplier<Nodes> pathDecisionApplier(
				flowTimeMatrixOffset);
			Relocation::Decision::DecisionApplier<Nodes> decisionApplier(
				fillDecisionApplier,
				pathDecisionApplier);
			DecisionMaking::FillGreedyAlgorithm<Nodes> fillGreedyAlgorithm(
				configuration.schedulingConfiguration.thresholdConfiguration);
			DecisionMaking::PathGreedyAlgorithm<Nodes> pathGreedyAlgorithm(
				fillGreedyAlgorithm,
				fillDecisionApplier,
				pathDecisionApplier);
			Scheduling::FlowRelocationScheduler<Nodes> scheduler(
				configuration.schedulingConfiguration.schedulingSpaceConfiguration,
				pathGreedyAlgorithm,
				decisionApplier);

			std::vector<std::vector<Relocation::RelocationOperation>> relocationOperations = scheduler.schedule(relocationModel, predictedWindow);

			for (size_t unit = 0; unit < relocationModel.relocationUnits.size(); unit++)
			{
				auto& relocationUnit = relocationModel.relocationUnits[unit];
				auto& operations = relocationOperations[unit];

				for (auto& operation : operations)
				{
					if (relocationUnit.timeUntilDestination >= 1)
						break;

					decisionApplier.check(relocationUnit, getCurrentState(), operation);
					decisionApplier.apply(relocationUnit, getCurrentState(), operation);
				}
			}

			for (auto& relocationUnit : relocationModel.relocationUnits)
				if (relocationUnit.timeUntilDestination >= 1)
					relocationUnit.timeUntilDestination--;

			timeFrame++;

			// forward simulation

			realFlowSimulation.step();
			predictedFlowSimulation.step();
			predictedFlowSimulation.getCurrentState() = realFlowSimulation.getCurrentState();
		}

		Model::FlowDistributionModel<Nodes> run(size_t timeFrames)
		{
			Model::FlowDistributionModel<Nodes> model;

			for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
			{
				model.timeFrames.push_back(getCurrentState());
				step();
			}

			return model;
		}
	};

}