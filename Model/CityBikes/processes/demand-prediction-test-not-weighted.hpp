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
	void testDemandPredictionNotWeighted(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto distanceFunction = Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::readData("../../Resources/processed/simple_demand_distance.distance");
		auto testDays = Data::Time::Utils::DaysReader::readData("../../Resources/configuration/test_examples.days");
		auto cumulativeDemandPrediction = Data::Demand::Utils::CumulativeDemandPredictionReader<Nodes>::readData("../../Resources/processed/predicted_demand.demand", timeFrames);

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
			// std::cout << Data::Time::Day::to_string(example.first) << std::endl;

			std::vector<Data::Flow::FlowAction> sameDayActions;

			{
				for (const Data::Flow::FlowInstance& instance : example.second)
					sameDayActions.push_back(Data::Flow::FlowAction(instance, 1.));
			}

			Data::Demand::CumulativeDemandPrediction<Nodes> sameDayActionsDemand = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(sameDayActions, timeFrames);

			std::array<double, Nodes> distances = Model::Prediction::DistanceAnalysis<Nodes>::computeSimpleDistance(
				cumulativeDemandPrediction.at(example.first),
				sameDayActionsDemand);

			double distance = 0;
			for (auto value : distances)
				distance += value;

			std::cout << distance / Nodes << "\t";

			std::cout << std::endl;
		}

		std::getchar();
	}
}