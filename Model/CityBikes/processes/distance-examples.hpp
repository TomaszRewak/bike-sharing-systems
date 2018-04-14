#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/demand/utils/cumulative-demand-prediction-reader.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"
#include "../../CityBikes.Data/day-distance/day-distance-function.hpp"
#include "../../CityBikes.Data/day-distance/utils/day-distance-functions-reader.hpp"

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void computeDistanceExamples()
	{
		size_t timeFrames = 288;
		size_t window = timeFrames / 6;

		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		Data::DayDistance::DayDistanceFunction<Nodes> distanceFunction;

		for (auto &dayA : examples)
		{
			std::cout << Data::Time::Day::to_string(dayA.first) << std::endl;
			Data::Demand::CumulativeDemandPrediction<Nodes> demandA = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(dayA.second, timeFrames);

			for (auto &dayB : examples)
			{
				Data::Demand::CumulativeDemandPrediction<Nodes> demandB = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(dayB.second, timeFrames);

				//std::array<double, Nodes> distances = Model::Prediction::DemandAnalysis<Nodes>::computeDistance(demandA, demandB, window);
				std::array<double, Nodes> distances = Model::Prediction::DemandAnalysis<Nodes>::computeSimpleDistance(demandA, demandB);

				for (size_t node = 0; node < Nodes; node++)
					distanceFunction[dayA.first][dayB.first][node] = distances[node];
			}
		}

		Data::DayDistance::Utils::DayDistanceFunctionReader<Nodes>::writeData(
			"../../Resources/processed/simple_demand_distance.distance",
			distanceFunction);
	}
}