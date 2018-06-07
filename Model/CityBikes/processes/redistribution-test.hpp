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
	void testRedistribution(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto flowMatrices = Data::FlowTime::Utils::FlowTimeMatricesReader::readData<Nodes>("../../Resources/processed/time_matrices.time", timeFrames);
		auto distanceFunctions = Data::DayDistance::Utils::DayDistanceFunctionReader::readData("../../Resources/learning/nn.model");
		auto simpleDistanceFunctions = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/learning/simple.model");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		Data::FillLevel::FillLevelPredictionFrame<Nodes> initialState;
		for (auto& station : initialState)
			station = 8;

		for (auto& example : examples)
		{
			std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> otherExamples = DataProcessing::Flow::Filtering::getOthers(examples, example.first);
			Data::FlowTime::FlowTimePrediction<Nodes>& flowTimePrediction = flowMatrices.begin()->second;

			std::cout << Data::Time::Day::to_string(example.first) << "\t";

			Model::FlowDistributionSimulation<Nodes> distributionSimulation(initialState, example.second);

			Data::Relocation::RelocationTeam relocationTeamWith0;
			Data::Relocation::RelocationTeam relocationTeamWith2;

			{

			}
			
			{
				relocationTeamWith2.push_back(Data::Relocation::RelocationUnit(0, 8, 10));
				relocationTeamWith2.push_back(Data::Relocation::RelocationUnit(20, 8, 10));
				relocationTeamWith2.push_back(Data::Relocation::RelocationUnit(40, 8, 10));
				relocationTeamWith2.push_back(Data::Relocation::RelocationUnit(60, 8, 10));
			}


			std::vector<Data::Flow::FlowAction> noActions;
			std::vector<Data::Flow::FlowAction> randomDayActions;
			std::vector<Data::Flow::FlowAction> otherDaysActions;
			std::vector<Data::Flow::FlowAction> selectedDaysActions;
			std::vector<Data::Flow::FlowAction> simplySelectedDaysActions;
			std::vector<Data::Flow::FlowAction> sameDayActions;

			{

			}

			{
				auto randomExample = otherExamples.begin();
				std::advance(randomExample, std::rand() % otherExamples.size());

				for (const Data::Flow::FlowInstance& instance : randomExample->second)
					randomDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
			}

			{
				for (auto& otherExample : otherExamples)
					for (const Data::Flow::FlowInstance& instance : otherExample.second)
						otherDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / otherExamples.size()));
			}

			{
				std::vector<std::pair<Data::Time::Day, double>> sortedDays;

				for (auto otherExample : otherExamples)
					sortedDays.push_back(std::make_pair(otherExample.first, distanceFunctions[example.first][otherExample.first]));

				std::sort(
					sortedDays.begin(),
					sortedDays.end(),
					[](auto a, auto b) { return a.second < b.second; }
				);

				size_t selectedInstances = 40;

				for (size_t i = 0; i < selectedInstances; i++)
					for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
						selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
			}
			
			{
				std::vector<std::pair<Data::Time::Day, double>> sortedDays;

				for (auto otherExample : otherExamples)
					sortedDays.push_back(std::make_pair(otherExample.first, simpleDistanceFunctions[example.first][otherExample.first]));

				std::sort(
					sortedDays.begin(),
					sortedDays.end(),
					[](auto a, auto b) { return a.second < b.second; }
				);

				size_t selectedInstances = 40;

				for (size_t i = 0; i < selectedInstances; i++)
					for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
						selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
			}

			{
				for (const Data::Flow::FlowInstance& instance : example.second)
					sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
			}

			Data::Demand::DemandPrediction<Nodes> noActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(noActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> randomDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(randomDayActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> otherDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(otherDaysActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> selectedDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> sameDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(sameDayActions, timeFrames);

			Data::Supply::SupplyPrediction<Nodes> noActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(noActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> randomDayActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(randomDayActions, timeFrames);
			//Data::Supply::SupplyPrediction<Nodes> otherDaysActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(otherDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> selectedDaysActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(selectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> sameDayActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(sameDayActions, timeFrames);

			Data::Supply::SupplyPrediction<Nodes> noActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(noActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> randomDayActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(randomDayActions, timeFrames);
			//Data::Supply::SupplyPrediction<Nodes> otherDaysActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(otherDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> selectedDaysActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(selectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> sameDayActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(sameDayActions, timeFrames);

			Redistribution::FlowRelocationSimulation<Nodes> florRelocationSimulation(
				4,
				0.1, 0.01,
				2, 8,
				20, 200
			);

			Data::FillLevel::FillLevelPrediction<Nodes> noActionsWithoutRelocationPrediction = florRelocationSimulation.simulate(
				distributionSimulation,
				noActionsFullSupply,
				noActionsDemand,
				noActionsFullSupply,
				relocationTeamWith0,
				flowTimePrediction,
				timeFrames);

			Data::FillLevel::FillLevelPrediction<Nodes> noActionsWithRelocationPrediction = florRelocationSimulation.simulate(
				distributionSimulation,
				noActionsFullSupply,
				noActionsDemand,
				noActionsFullSupply,
				relocationTeamWith2,
				flowTimePrediction,
				timeFrames);

			Data::FillLevel::FillLevelPrediction<Nodes> selectedDaysActionsWithRelocationPrediction = florRelocationSimulation.simulate(
				distributionSimulation,
				selectedDaysActionsFullSupply,
				selectedDaysActionsDemand,
				selectedDaysActionsFullSupply,
				relocationTeamWith2,
				flowTimePrediction,
				timeFrames);

			Data::FillLevel::FillLevelPrediction<Nodes> sameDayActionsWithRelocationPrediction = florRelocationSimulation.simulate(
				distributionSimulation,
				sameDayActionsFullSupply,
				sameDayActionsDemand,
				sameDayActionsFullSupply,
				relocationTeamWith2,
				flowTimePrediction,
				timeFrames);

			Data::FillLevel::FillLevelPrediction<Nodes> realPrediction = distributionSimulation.run(timeFrames);

			std::cout <<
				DataProcessing::Validation::AvailabilityValidation<Nodes>::getTimeOfUnavailability(realPrediction) << "\t" <<
				DataProcessing::Validation::AvailabilityValidation<Nodes>::getTimeOfUnavailability(noActionsWithoutRelocationPrediction) << "\t" <<
				DataProcessing::Validation::AvailabilityValidation<Nodes>::getTimeOfUnavailability(noActionsWithRelocationPrediction) << "\t" <<
				DataProcessing::Validation::AvailabilityValidation<Nodes>::getTimeOfUnavailability(selectedDaysActionsWithRelocationPrediction) << "\t" <<
				DataProcessing::Validation::AvailabilityValidation<Nodes>::getTimeOfUnavailability(sameDayActionsWithRelocationPrediction) << std::endl;
		}

		std::getchar();
	}
}