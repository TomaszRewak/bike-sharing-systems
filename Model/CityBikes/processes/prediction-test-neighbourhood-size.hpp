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
	void testPredictionNeighbourhoodSize()
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

		size_t windowSizes[]{ 1, 2, 3, 4, 5, 6, 8, 10, 12, 15, 18, 24, 36, 72, 140 };

		size_t selectedInstancesSizes[]{
			1,
			2,
			3,
			4,
			5,
			7,
			10,
			14,
			19,
			26,
			36,
			40,
			59,
			85,
			121,
			161,
			180
		};

		for (size_t window : windowSizes)
		{
			std::cout << "=" << window << "=" << "\t";

			for (size_t selectedInstances : selectedInstancesSizes)
			{
				double totalOffset = 0;

				for (auto& example : testExamples)
				{
					//std::cout << Data::Time::Day::to_string(example.first) << std::endl;

					//std::vector<Data::Flow::FlowAction> noActions;
					//std::vector<Data::Flow::FlowAction> otherDaysActions;
					//std::vector<Data::Flow::FlowAction> simplySelectedDaysActions;
					std::vector<Data::Flow::FlowAction> selectedDaysActions;
					//std::vector<Data::Flow::FlowAction> sameDayActions;

					{

					}

					//{
					//	for (auto& otherExample : otherExamples)
					//		for (const Data::Flow::FlowInstance& instance : otherExample.second)
					//			otherDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / otherExamples.size()));
					//}

					{
						for (size_t node = 0; node < Nodes - 1; node++)
						{
							std::vector<std::pair<Data::Time::Day, double>> sortedDays;

							for (auto otherExample : otherExamples)
								sortedDays.push_back(std::make_pair(otherExample.first, nnDistanceFunctions[example.first][otherExample.first][node]));

							std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

							for (size_t i = 0; i < selectedInstances; i++)
								for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
									if (instance.source.node == node)
										selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
						}
					}

					//{
					//	size_t selectedInstances = 40;
					//
					//	for (size_t node = 0; node < Nodes; node++)
					//	{
					//		std::vector<std::pair<Data::Time::Day, double>> sortedDays;
					//
					//		for (auto otherExample : otherExamples)
					//			sortedDays.push_back(std::make_pair(otherExample.first, simpleDistanceFunctions[example.first][otherExample.first][node]));
					//
					//		std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });
					//
					//		for (size_t i = 0; i < selectedInstances; i++)
					//			for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
					//				if (instance.source.node == node)
					//					simplySelectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
					//	}
					//}

					//{
					//	for (const Data::Flow::FlowInstance& instance : example.second)
					//		sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
					//}

					Data::Demand::DemandPrediction<Nodes> demandPrediction = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(selectedDaysActions, timeFrames);
					Data::Supply::SupplyPrediction<Nodes> supplyPrediction = Model::Prediction::SupplyAnalysis<Nodes>::computeFullSupply(selectedDaysActions, timeFrames);

					Model::FlowDistributionSimulation<Nodes> distributionSimulation(initialState, example.second);
					double offset = 0;

					for (size_t timeFrame = 0; timeFrame < timeFrames - window; timeFrame++)
					{
						Data::FillLevel::FillLevelPredictionFrame<Nodes> currentState = distributionSimulation.getCurrentState();
						std::vector<Data::Flow::FlowTarget> ongoingFlow = DataProcessing::Flow::Casting::getOngoingFlow(distributionSimulation.getOngoingInstances());

						size_t startTimeFrame = distributionSimulation.getTimeFrame();
						size_t endTimeFrame = startTimeFrame + window;

						Model::FlowDistributionSimulation<Nodes> distributionSimulationCopy = distributionSimulation;

						Data::FillLevel::FillLevelPrediction<Nodes> realPrediction = distributionSimulationCopy.run(endTimeFrame);
						Data::FillLevel::FillLevelPrediction<Nodes> ownPrediction = Model::FlowPredictionSimulation<Nodes>::predict(
							currentState, ongoingFlow,
							supplyPrediction,
							demandPrediction,
							supplyPrediction,
							startTimeFrame, endTimeFrame);

						offset += DataProcessing::FillLevel::Error<Nodes>::computeOffset(realPrediction, ownPrediction);

						distributionSimulation.step();
					}

					totalOffset += offset / (timeFrames - window);
				}

				std::cout << totalOffset / testExamples.size() << "\t";
			}

			std::cout << std::endl;
		}

		std::getchar();
	}
}