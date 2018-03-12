#pragma once

#include "flow-relocation-model.hpp"
#include "../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "scheduling/flow-relocation-scheduler.hpp"
#include "configuration/simulation-configuration.hpp"

namespace CityBikes::Flow
{
	template<size_t Nodes>
	class FlowRelocationSimulation
	{
	private:
		const Configuration::SimulationConfiguration& configuration;

		const Data::FlowTime::FlowTimeMatrix<Nodes>& flowTimeMatrix;

		Model::FlowDistributionModelSimulation<Nodes> realFlowSimulation;
		Model::FlowDistributionModelSimulation<Nodes> predictedFlowSimulation;

		size_t timeFrame = 0;
		FlowRelocationModel relocationModel;

	public:
		FlowRelocationSimulation(
			Model::Structure::NetworkState<Nodes> initialState,
			FlowRelocationModel relocationModel,
			const Configuration::SimulationConfiguration& configuration,
			const Data::FlowTime::FlowTimeMatrix<Nodes>& flowTimeMatrix
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
			auto predictedWindow = predictionWindowSimulation.run(configuration.relocationSchedulingConfiguration.predictionWindow);

			Data::FlowTime::FlowTimeMatrixOffset<Nodes> flowTimeMatrixOffset(
				flowTimeMatrix,
				timeFrame);
			Scheduling::FlowRelocationScheduler<Nodes> scheduler(
				flowTimeMatrixOffset,
				configuration.schedulingConfiguration);

			auto relocationOperations = scheduler.schedule(relocationModel, predictedWindow);

			applyRelocationOperations(relocationOperations);

			for (auto& relocationUnit : relocationModel.relocationUnits)
				if (relocationUnit.currentOperation.remainingTime > 0)
					relocationUnit.currentOperation.remainingTime--;

			timeFrame++;

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

	private:
		void applyRelocationOperations(const std::vector<std::list<Relocation::RelocationOperation>>& operations)
		{
			for (size_t unit = 0; unit < relocationModel.relocationUnits.size(); unit++)
			{
				auto& relocationUnit = relocationModel.relocationUnits[unit];
				auto unitOperations = operations[unit];

				while (relocationUnit.currentOperation.remainingTime == 0)
				{
					applyRelocationOperation(relocationUnit);

					if (unitOperations.size() > 0)
					{
						relocationUnit.schedule(unitOperations.front());
						unitOperations.pop_front();
					}
					else break;
				}
			}
		}

		void applyRelocationOperation(Relocation::RelocationUnit& relocationUnit)
		{
			Model::Structure::NetworkState<Nodes>& currentState = getCurrentState();

			int fillChange = relocationUnit.currentOperation.destinationFillChange;
			size_t destination = relocationUnit.currentOperation.destination;

			if (fillChange > 0)
			{
				if (fillChange > relocationUnit.currentLoad)
					throw "You cannot give more then you have";
			}
			if (fillChange < 0)
			{
				if (relocationUnit.currentLoad - fillChange > relocationUnit.maxLoad)
					throw "You cannot get more then you can have";

				fillChange = std::max((double)fillChange, -currentState.nodes[destination].value);
			}

			relocationUnit.currentLoad -= fillChange;
			currentState.nodes[destination].add(fillChange);
			relocationUnit.currentOperation.destinationFillChange = 0;
		}
	};

}