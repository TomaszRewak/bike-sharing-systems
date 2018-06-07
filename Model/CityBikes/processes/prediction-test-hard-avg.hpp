#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Data/time/utils/days_reader.hpp"
#include "../../CityBikes.Data/day-distance/utils/day-distance-functions-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.Model/hard-flow-prediction-simulation.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"
#include "../../CityBikes.Model/prediction/supply-analysis.hpp"
#include "../../CityBikes.DataProcessing/fill-level/error.hpp"
#include "../../CityBikes.DataProcessing/flow/casting.hpp"
#include "../../CityBikes.DataProcessing/flow/filtering.hpp"
#include "../../CityBikes.DataProcessing/demand/demand-prediction-processing.hpp"
#include "../../CityBikes.DataProcessing/fill-level/transforming.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testPredictionHardAvg()
	{
		const size_t timeFrames = 144;

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
			std::cout << Data::Time::Day::to_string(example.first) << std::endl;

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

				for (size_t node = 0; node < Nodes - 1; node++)
				{
					std::vector<std::pair<Data::Time::Day, double>> sortedDays;

					for (auto otherExample : otherExamples)
						sortedDays.push_back(std::make_pair(otherExample.first, nnDistanceFunctions[example.first][otherExample.first][node]));

					std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

					double minDistance = sortedDays[0].second;
					double maxDistance = sortedDays[selectedInstances - 1].second + 0.0001;

					for (auto& sortedDay : sortedDays)
						sortedDay.second = 1 - (sortedDay.second - minDistance) / (maxDistance - minDistance);

					double distanceSum = 0;
					for (size_t i = 0; i < selectedInstances; i++)
						distanceSum += sortedDays[i].second;

					for (size_t i = 0; i < selectedInstances; i++)
						for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
							if (instance.source.node == node)
								selectedDaysActions.push_back(Data::Flow::FlowAction(instance, sortedDays[i].second / distanceSum));
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
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(noActions, timeFrames))),
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(otherDaysActions, timeFrames))),
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(simplySelectedDaysActions, timeFrames))),
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(selectedDaysActions, timeFrames))),
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(sameDayActions, timeFrames))),
				//Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(DataProcessing::Demand::DemandPredictionProcessing<Nodes>::discretize(cumulativeDemand.at(example.first))),
				Model::Prediction::DemandProcessing<Nodes>::computeDemand(Model::Prediction::DemandProcessing<Nodes>::computeGlobalDemand(Model::Prediction::DemandAnalysis<Nodes>::decumulateDemand(cumulativeDemand.at(example.first))), Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames))
			};

			Data::Supply::SupplyPrediction<Nodes> supplyPrediction[] = {
				Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(noActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(otherDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(simplySelectedDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(selectedDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeAvgSupply(sameDayActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(noActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(otherDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(simplySelectedDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(selectedDaysActions, timeFrames),
				Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(sameDayActions, timeFrames),
			};

			size_t window = timeFrames / 6;

			for (size_t i = 0; i < 1; i++)
			{
				for (size_t j = 0; j < 10; j++)
				{
					Model::FlowDistributionSimulation<Nodes> distributionSimulation(initialState, example.second);
					double offset = 0;

					const size_t attempts = 20;

					for (size_t timeFrame = 0; timeFrame < timeFrames - window; timeFrame++)
					{
						Data::FillLevel::FillLevelPredictionFrame<Nodes> currentState = distributionSimulation.getCurrentState();
						std::vector<Data::Flow::FlowTarget> ongoingFlow = DataProcessing::Flow::Casting::getOngoingFlow(distributionSimulation.getOngoingInstances());

						size_t startTimeFrame = distributionSimulation.getTimeFrame();
						size_t endTimeFrame = startTimeFrame + window;

						std::vector<Data::FillLevel::FillLevelPrediction<Nodes>> ownPredictions;
						for (size_t attempt = 0; attempt < attempts; attempt++)
						{
							ownPredictions.push_back(
								Model::HardFlowPredictionSimulation<Nodes>::predict(
									currentState, ongoingFlow,
									supplyPrediction[j],
									demandPredictions[i],
									supplyPrediction[j],
									startTimeFrame, endTimeFrame)
							);
						}

						Model::FlowDistributionSimulation<Nodes> distributionSimulationCopy = distributionSimulation;
						Data::FillLevel::FillLevelPrediction<Nodes> realPrediction = distributionSimulationCopy.run(endTimeFrame);

						Data::FillLevel::FillLevelPrediction<Nodes> ownPrediction = DataProcessing::FillLevel::FillLevelProcessing<Nodes>::average(
							ownPredictions
						);

						offset += DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, ownPrediction);

						distributionSimulation.step();
					}

					std::cout << offset / (timeFrames - window) << "\t";
				}

				std::cout << std::endl;
			}
		}

		std::getchar();
	}
}