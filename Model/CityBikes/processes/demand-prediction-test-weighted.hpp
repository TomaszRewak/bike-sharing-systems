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
	void testDemandPredictionWeighted(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto distanceFunction = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/learning/nn.model");
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

			for (size_t selectedInstances = 1; selectedInstances < otherExamples.size(); selectedInstances++)
			{

				std::vector<Data::Flow::FlowAction> selectedDaysActions;
				std::vector<Data::Flow::FlowAction> sameDayActions;

				{
					for (size_t node = 0; node < Nodes; node++)
					{
						std::vector<std::pair<Data::Time::Day, double>> sortedDays;

						for (auto otherExample : otherExamples)
							sortedDays.push_back(std::make_pair(otherExample.first, distanceFunction[example.first][otherExample.first][node]));

						std::sort(sortedDays.begin(), sortedDays.end(), [](auto a, auto b) { return a.second < b.second; });

						if (selectedInstances == 1)
						{
							for (size_t i = 0; i < selectedInstances; i++)
								for (const Data::Flow::FlowInstance& instance : otherExamples[sortedDays[i].first])
									if (instance.source.node == node)
										selectedDaysActions.push_back(Data::Flow::FlowAction(instance, 1. / selectedInstances));
						}
						else
						{
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
				}

				{
					for (const Data::Flow::FlowInstance& instance : example.second)
						sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
				}

				Data::Demand::CumulativeDemandPrediction<Nodes> selectedDaysActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(selectedDaysActions, timeFrames);
				Data::Demand::CumulativeDemandPrediction<Nodes> sameDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(sameDayActions, timeFrames);

				std::array<double, Nodes> distances = Model::Prediction::DistanceAnalysis<Nodes>::computeSimpleDistance(
					selectedDaysActionsDemand,
					sameDayActionsDemand);

				double distance = 0;
				for (auto value : distances)
					distance += value;

				std::cout << distance / Nodes << "\t";
			}
			std::cout << std::endl;
		}

		std::getchar();
	}
}