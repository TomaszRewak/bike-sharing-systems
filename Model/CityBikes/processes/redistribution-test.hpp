#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Flow/flow-relocation-simulation.hpp"
#include "../../CityBikes.DataProcessing/validation/availability-validation.hpp"

#include <iostream>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	Model::FlowDistributionModel<Nodes> testRedistributionForConfiguration(
		size_t timeFrames,
		Model::Structure::NetworkState<Nodes> initialState,
		Flow::FlowRelocationModel initialModel,
		const Flow::Configuration::SimulationConfiguration& simulationConfiguration,
		const Data::FlowTime::FlowTimePrediction<Nodes> flowMatrix
	)
	{
		Flow::FlowRelocationSimulation<Nodes> redistributionSimulation(
			initialState,
			initialModel,
			simulationConfiguration,
			flowMatrix
		);

		return redistributionSimulation.run(timeFrames);
	}

	template<size_t Nodes>
	void testRedistribution(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto flowMatrices = Data::FlowTime::Utils::FlowTimeMatricesReader::readData<Nodes>("../../Resources/processed/time_matrices.time", timeFrames);
		auto distanceFunctions = Data::Distance::Utils::DistanceFunctionsReader::readData("../../Resources/learning/nn.model");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		size_t exampleNumber = 0;

		for (const auto& example : examples)
		{
			exampleNumber++;

			if (exampleNumber < 50)
				continue;

			std::vector<Model::Data::FlowAction> predictionActions;
			{
				for (auto& otherExample : examples)
					if (otherExample.first != example.first)
						for (auto& flowInstance : otherExample.second)
							predictionActions.push_back(Model::Data::FlowAction(flowInstance, 1. / (examples.size() - 1)));
			}

			std::vector<Model::Data::FlowAction> bestPredictionActions;
			{
				size_t bestPredictionExamplesNumber = 50;

				std::vector<std::pair<float, Data::Common::Day>> sortedDays;

				for (auto otherExample : examples)
					if (otherExample.first != example.first)
						sortedDays.push_back(std::make_pair(distanceFunctions[example.first][otherExample.first], otherExample.first));

				std::sort(
					sortedDays.begin(),
					sortedDays.end(),
					[](auto a, auto b) { return a.first < b.first; }
				);


				for (size_t i = 0; i < bestPredictionExamplesNumber; i++)
				{
					for (auto& flowInstance : examples[sortedDays[i].second])
						predictionActions.push_back(Model::Data::FlowAction(flowInstance, 1. / bestPredictionExamplesNumber));
				}
			}

			std::vector<Model::Data::FlowAction> flowActions;
			{
				for (auto& flowInstance : example.second)
					flowActions.push_back(Model::Data::FlowAction(flowInstance, 1.));
			}

			std::vector<Model::Data::FlowAction> noActions;
			{
			}

			Model::Structure::NetworkState<Nodes> initialState;
			for (auto& station : initialState.nodes)
				station.value = 8;

			Flow::FlowRelocationModel initialModelNo(0, 0, 10, 10);
			Flow::FlowRelocationModel initialModelTwo(2, 0, 10, 10);

			Flow::Configuration::SimulationConfiguration simulationConfigurationPrediction(
				Flow::Configuration::RelocationSchedulingConfiguration(30),
				Flow::Configuration::SchedulingConfiguration(
					Flow::Configuration::ThresholdConfiguration(2, 10),
					Flow::Configuration::OperationTimeConfiguration(5, 0.5),
					Flow::Configuration::SchedulingSpaceConfiguration(20, 25)
				),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(flowActions),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(predictionActions)
			);

			Flow::Configuration::SimulationConfiguration simulationConfigurationBestPrediction(
				Flow::Configuration::RelocationSchedulingConfiguration(30),
				Flow::Configuration::SchedulingConfiguration(
					Flow::Configuration::ThresholdConfiguration(2, 10),
					Flow::Configuration::OperationTimeConfiguration(5, 0.5),
					Flow::Configuration::SchedulingSpaceConfiguration(20, 25)
				),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(flowActions),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(bestPredictionActions)
			);

			Flow::Configuration::SimulationConfiguration simulationConfigurationFlow(
				Flow::Configuration::RelocationSchedulingConfiguration(30),
				Flow::Configuration::SchedulingConfiguration(
					Flow::Configuration::ThresholdConfiguration(2, 10),
					Flow::Configuration::OperationTimeConfiguration(5, 0.5),
					Flow::Configuration::SchedulingSpaceConfiguration(20, 25)
				),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(flowActions),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(flowActions)
			);

			Flow::Configuration::SimulationConfiguration simulationConfigurationNo(
				Flow::Configuration::RelocationSchedulingConfiguration(30),
				Flow::Configuration::SchedulingConfiguration(
					Flow::Configuration::ThresholdConfiguration(2, 10),
					Flow::Configuration::OperationTimeConfiguration(8, 0.5),
					Flow::Configuration::SchedulingSpaceConfiguration(20, 25)
				),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(flowActions),
				Model::Configuration::FlowDistributionModelSimulationConfiguration(noActions)
			);

			auto modelTwoFlow = testRedistributionForConfiguration(timeFrames, initialState, initialModelTwo, simulationConfigurationFlow, flowMatrices.begin()->second);
			auto modelNo = testRedistributionForConfiguration(timeFrames, initialState, initialModelNo, simulationConfigurationPrediction, flowMatrices.begin()->second);
			auto modelTwoPrediction = testRedistributionForConfiguration(timeFrames, initialState, initialModelTwo, simulationConfigurationPrediction, flowMatrices.begin()->second);
			auto modelTwoBestPrediction = testRedistributionForConfiguration(timeFrames, initialState, initialModelTwo, simulationConfigurationBestPrediction, flowMatrices.begin()->second);
			auto modelTwoNo = testRedistributionForConfiguration(timeFrames, initialState, initialModelTwo, simulationConfigurationNo, flowMatrices.begin()->second);

			std::cout
				<< std::round(100 * DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(modelNo)) / 100 << "\t"
				<< std::round(100 * DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(modelTwoNo)) / 100 << "\t"
				<< std::round(100 * DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(modelTwoPrediction)) / 100 << "\t"
				<< std::round(100 * DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(modelTwoBestPrediction)) / 100 << "\t"
				<< std::round(100 * DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(modelTwoFlow)) / 100 << std::endl;
		}

		std::getchar();
	}
}