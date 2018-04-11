#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/demand/utils/cumulative-demand-prediction-reader.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void computeDemandExamples()
	{
		size_t timeFrames = 1440;

		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		std::map<Data::Time::Day, Data::Demand::CumulativeDemandPrediction<Nodes>> demands;

		for (auto &day : examples)
		{
			Data::Demand::CumulativeDemandPrediction<Nodes> demand = Model::Prediction::DemandAnalysis<Nodes>::computeCumulativeDemand(day.second, timeFrames);

			demands.emplace(day.first, demand);
		}

		Data::Demand::Utils::CumulativeDemandPredictionReader<Nodes>::writeData(
			"../../Resources/processed/examples_demand.demand",
			demands,
			144);
	}
}