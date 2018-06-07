#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Data/day-distance/utils/day-distance-functions-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.Model/flow-prediction-simulation.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"
#include "../../CityBikes.Model/prediction/supply-analysis.hpp"
#include "../../CityBikes.DataProcessing/fill-level/error.hpp"
#include "../../CityBikes.Redistribution/flow-relocation-simulation.hpp"
#include "../../CityBikes.DataProcessing/validation/availability-validation.hpp"

#include <iostream>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testRedistributionClients(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto flowMatrices = Data::FlowTime::Utils::FlowTimeMatricesReader::readData<Nodes>("../../Resources/processed/time_matrices.time", timeFrames);
		auto distanceFunctions = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/learning/nn.model");
		auto simpleDistanceFunctions = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/learning/simple.model");
		auto cumulativeDemand = Data::Demand::Utils::CumulativeDemandPredictionReader<Nodes>::readData("../../Resources/processed/predicted_demand.demand", timeFrames);
		auto testDays = Data::Time::Utils::DaysReader::readData("../../Resources/configuration/test_examples.days");

		auto allExamples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);
		std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> testExamples;
		std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> otherExamples;

		for (auto const&[day, example] : allExamples)
			if (testDays.count(day))
				testExamples[day] = example;
			else
				otherExamples[day] = example;

		Data::FillLevel::FillLevelPredictionFrame<Nodes> initialState;
		for (auto& station : initialState)
			station = 8;

		size_t i = 0;

		for (auto& example : testExamples)
		{
			if (i < 11)
			{
				i++;
				continue;
			}

			Data::FlowTime::FlowTimePrediction<Nodes>& flowTimePrediction = flowMatrices.begin()->second;
			Model::FlowDistributionSimulation<Nodes> distributionSimulation(initialState, example.second);

			Data::Relocation::RelocationTeam relocationTeam;

			{
				relocationTeam.push_back(Data::Relocation::RelocationUnit(0, 8, 10));
				relocationTeam.push_back(Data::Relocation::RelocationUnit(20, 8, 10));
			}


			std::vector<Data::Flow::FlowAction> noActions;
			std::vector<Data::Flow::FlowAction> otherDaysActions;
			std::vector<Data::Flow::FlowAction> simplySelectedDaysActions;
			std::vector<Data::Flow::FlowAction> selectedDaysActions;
			std::vector<Data::Flow::FlowAction> sameDayActions;

			{

			}

			{
				for (auto& otherExample : otherExamples)
					for (const Data::Flow::FlowInstance& instance : otherExample.second)
						otherDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / otherExamples.size()));
			}

			{
				size_t selectedInstances = 40;

				for (size_t node = 0; node < Nodes; node++)
				{
					std::vector<std::pair<Data::Time::Day, double>> sortedDays;

					for (auto otherExample : otherExamples)
						sortedDays.push_back(std::make_pair(otherExample.first, distanceFunctions[example.first][otherExample.first][node]));

					std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

					for (size_t i = 0; i < selectedInstances; i++)
						for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
							if (instance.source.node == node)
								selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
				}
			}

			{
				size_t selectedInstances = 40;

				for (size_t node = 0; node < Nodes; node++)
				{
					std::vector<std::pair<Data::Time::Day, double>> sortedDays;

					for (auto otherExample : otherExamples)
						sortedDays.push_back(std::make_pair(otherExample.first, simpleDistanceFunctions[example.first][otherExample.first][node]));

					std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

					for (size_t i = 0; i < selectedInstances; i++)
						for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
							if (instance.source.node == node)
								simplySelectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
				}
			}

			{
				for (const Data::Flow::FlowInstance& instance : example.second)
					sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
			}

			Data::Demand::DemandPrediction<Nodes> demandPredictions[] = {
				Model::Prediction::DemandAnalysis<Nodes>::computeDemand(noActions, timeFrames),
				Model::Prediction::DemandAnalysis<Nodes>::computeDemand(otherDaysActions, timeFrames),
				Model::Prediction::DemandAnalysis<Nodes>::computeDemand(simplySelectedDaysActions, timeFrames),
				Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(cumulativeDemand.at(example.first)),
				Model::Prediction::DemandProcessing<Nodes>::computeDemand(Model::Prediction::DemandProcessing<Nodes>::computeGlobalDemand(Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(cumulativeDemand.at(example.first))), Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames)),
				Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames)
			};
			Data::Supply::SupplyPrediction<Nodes> supplyPrediction = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(selectedDaysActions, timeFrames);

			Data::Demand::DemandPrediction<Nodes> sameDayDemandPrediction = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(sameDayActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> sameDaySupplyPrediction = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(sameDayActions, timeFrames);

			Redistribution::FlowRelocationSimulation<Nodes> flowRelocationSimulation(
				6,
				0.1, 0.01,
				2, 14,
				5, 200
			);

			for (const auto& demandPrediction : demandPredictions)
			{
				Data::FillLevel::FillLevelPrediction<Nodes> relocationPrediction = flowRelocationSimulation.simulate(
					distributionSimulation,
					supplyPrediction,
					demandPrediction,
					supplyPrediction,
					relocationTeam,
					flowTimePrediction,
					timeFrames);

				std::cout << flowRelocationSimulation.getUnhandledCustomers() << "\t";
			}

			Data::FillLevel::FillLevelPrediction<Nodes> sameDayRelocationPrediction = flowRelocationSimulation.simulate(
				distributionSimulation,
				sameDaySupplyPrediction,
				sameDayDemandPrediction,
				sameDaySupplyPrediction,
				relocationTeam,
				flowTimePrediction,
				timeFrames);

			std::cout << flowRelocationSimulation.getUnhandledCustomers() << "\t";

			Model::FlowDistributionSimulation<Nodes> distributionSimulationCopy = distributionSimulation;
			Data::FillLevel::FillLevelPrediction<Nodes> realPrediction = distributionSimulationCopy.run(timeFrames);

			std::cout << distributionSimulationCopy.getUnhandledCustomers() << std::endl;
		}

		std::getchar();
	}
}