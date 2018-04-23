#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Data/time/utils/days_reader.hpp"
#include "../../CityBikes.Data/day-distance/utils/day-distance-functions-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.Model/flow-prediction-simulation.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"
#include "../../CityBikes.Model/prediction/supply-analysis.hpp"
#include "../../CityBikes.DataProcessing/fill-level/error.hpp"
#include "../../CityBikes.DataProcessing/flow/casting.hpp"
#include "../../CityBikes.DataProcessing/flow/filtering.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testPrediction(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto nnDistanceFunctions = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/learning/nn.model");
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
			station = 1000;

		for (auto& example : testExamples)
		{
			std::cout << Data::Time::Day::to_string(example.first) << "\t";

			Model::FlowDistributionSimulation<Nodes> distributionSimulation(initialState, example.second);
			distributionSimulation.run(timeFrames / 2);

			std::vector<Data::Flow::FlowAction> noActions;
			std::vector<Data::Flow::FlowAction> randomDayActions;
			std::vector<Data::Flow::FlowAction> otherDaysActions;
			std::vector<Data::Flow::FlowAction> simplySelectedDaysActions;
			std::vector<Data::Flow::FlowAction> selectedDaysActions;
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
				for (size_t node = 0; node < Nodes; node++)
				{
					std::vector<std::pair<Data::Time::Day, double>> sortedDays;

					for (auto otherExample : otherExamples)
						sortedDays.push_back(std::make_pair(otherExample.first, simpleDistanceFunctions[example.first][otherExample.first][node]));

					std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

					size_t selectedInstances = 100;

					for (size_t i = 0; i < selectedInstances; i++)
						for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
							if (instance.source.node == node)
								simplySelectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
				}
			}

			{
				for (size_t node = 0; node < Nodes; node++)
				{
					std::vector<std::pair<Data::Time::Day, double>> sortedDays;

					for (auto otherExample : otherExamples)
						sortedDays.push_back(std::make_pair(otherExample.first, nnDistanceFunctions[example.first][otherExample.first][node]));

					std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

					size_t selectedInstances = 100;

					for (size_t i = 0; i < selectedInstances; i++)
						for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
							if (instance.source.node == node)
								selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
				}
			}

			{
				for (const Data::Flow::FlowInstance& instance : example.second)
					sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
			}

			Data::Demand::DemandPrediction<Nodes> noActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(noActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> randomDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(randomDayActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> otherDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(otherDaysActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> simplySelectedDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(simplySelectedDaysActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> selectedDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> sameDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(sameDayActions, timeFrames);
			Data::Demand::DemandPrediction<Nodes> nnPredictedDemend = Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(cumulativeDemand.at(example.first));

			Data::Supply::SupplyPrediction<Nodes> noActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(noActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> randomDayActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(randomDayActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> otherDaysActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(otherDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> simplySelectedDaysActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(simplySelectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> selectedDaysActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(selectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> sameDayActionsAvgSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(sameDayActions, timeFrames);

			Data::Supply::SupplyPrediction<Nodes> noActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(noActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> randomDayActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(randomDayActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> otherDaysActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(otherDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> simplySelectedDaysActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(simplySelectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> selectedDaysActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(selectedDaysActions, timeFrames);
			Data::Supply::SupplyPrediction<Nodes> sameDayActionsFullSupply = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(sameDayActions, timeFrames);

			Data::FillLevel::FillLevelPredictionFrame<Nodes> currentState = distributionSimulation.getCurrentState();
			std::vector<Data::Flow::FlowTarget> ongoingFlow = DataProcessing::Flow::Casting::getOngoingFlow(distributionSimulation.getOngoingInstances());

			size_t startTimeFrame = distributionSimulation.getTimeFrame();
			size_t endTimeFrame = timeFrames;

			Data::FillLevel::FillLevelPrediction<Nodes> noActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				noActionsFullSupply,
				noActionsDemand,
				noActionsFullSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> otherDaysActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				otherDaysActionsAvgSupply,
				otherDaysActionsDemand,
				otherDaysActionsAvgSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> simplySelectedDaysDemandActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				simplySelectedDaysActionsAvgSupply,
				simplySelectedDaysActionsDemand,
				simplySelectedDaysActionsAvgSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> selectedDaysDemandActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				selectedDaysActionsAvgSupply,
				selectedDaysActionsDemand,
				selectedDaysActionsAvgSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> selectedDaysActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				selectedDaysActionsAvgSupply,
				selectedDaysActionsDemand,
				selectedDaysActionsFullSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> sameDayActionsPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				sameDayActionsFullSupply,
				sameDayActionsDemand,
				sameDayActionsFullSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> nnPredictionPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
				currentState, ongoingFlow,
				otherDaysActionsFullSupply,
				nnPredictedDemend,
				otherDaysActionsFullSupply,
				startTimeFrame, endTimeFrame);

			Data::FillLevel::FillLevelPrediction<Nodes> realPrediction = distributionSimulation.run(endTimeFrame);

			std::cout <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, noActionsPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, simplySelectedDaysDemandActionsPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, nnPredictionPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, otherDaysActionsPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, selectedDaysDemandActionsPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, selectedDaysActionsPrediction) << "\t" <<
				DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, sameDayActionsPrediction) << std::endl;
		}

		std::getchar();
	}
}