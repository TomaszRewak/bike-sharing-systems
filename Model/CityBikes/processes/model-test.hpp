#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.Model/flow-prediction-simulation.hpp"
#include "../../CityBikes.DataProcessing/fill-level/integrity.hpp"
#include "../../CityBikes.Data/fill-level/utils/fill-level-prediction-reader.hpp"

#include <iostream>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testModel(size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		Data::FillLevel::FillLevelPredictionFrame<Nodes> initialState;
		for (auto& station : initialState)
			station = initialStationSize;

		std::map<Data::Time::Day, Data::FillLevel::FillLevelPrediction<Nodes>> predictions;

		for (auto& example : examples)
		{
			std::cout << Data::Time::Day::to_string(example.first) << std::endl;

			Model::FlowDistributionSimulation<Nodes> simulation(initialState, example.second);

			auto prediction = simulation.run(timeFrames);

			std::cout << DataProcessing::FillLevel::Integrity<Nodes>::computeTotalNumber(*prediction.begin()) << std::endl;
			std::cout << DataProcessing::FillLevel::Integrity<Nodes>::computeTotalNumber(*prediction.rbegin()) << std::endl;

			predictions.emplace(example.first, prediction);
		}

		Data::FillLevel::Utils::FlowTimeMatricesReader::writeData("../../Resources/results/test_prediction.pred", predictions);

		std::getchar();
	}
}