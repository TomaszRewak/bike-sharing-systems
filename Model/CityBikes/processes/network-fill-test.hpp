#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"
#include "../../CityBikes.Flow/filling/network-filling-matrix.hpp"

#include <iostream>

namespace CityBikes::Processes
{
	void testNetworkFill(size_t stations, size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		Model::Structure::NetworkState initialState(stations);
		for (auto& station : initialState.nodes)
			station.value = initialStationSize;

		std::vector<Model::Data::FlowAction> actions;
		for (size_t i = 0; i < examplesNumber; i++)
		{
			size_t day = std::rand() % examples.size();

			for (auto& flowInstance : examples[day].flowInstances)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1. / examplesNumber));
		}

		Model::FlowDistributionModelSimulation simulation(actions);

		auto model = simulation.simulate(initialState, 0, timeFrames, true);

		std::cout << Test::ModelIntegrity::computeTotalNumber(*model.timeFrames.begin()) << std::endl;
		std::cout << Test::ModelIntegrity::computeTotalNumber(*model.timeFrames.rbegin()) << std::endl;

		Flow::Filling::NetworkFillingMatrix fillingMatrix(model);

		//ResultData::Predictions::PredictionWriter::writeData(model, "../../Resources/results/test_prediction.pred");

		std::getchar();
	}
}