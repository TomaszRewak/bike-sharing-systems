#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.DataProcessing/prediction/prediction-offset.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Data/features/features-reader.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	Model::FlowDistributionModel testPredictionPrepareModel(std::vector<DataProcessing::Rides::Structure::RideDayGroup> examples, size_t stations, size_t timeFrames, size_t initialStationSize)
	{
		Model::Structure::NetworkState initialState(stations);
		for (auto& station : initialState.nodes)
			station.value = initialStationSize;

		std::vector<Model::Data::FlowAction> actions;
		for (auto& example : examples)
			for (auto& flowInstance : example.flowInstances)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1. / examples.size()));

		Model::FlowDistributionModelSimulation simulation(actions);

		return simulation.simulate(initialState, 0, timeFrames, false);
	}

	void testPrediction(size_t stations, size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto features = Data::Features::FeaturesReader::readData("../../Resources/processed/features.features");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		for (auto& example : examples)
		{
			std::vector<DataProcessing::Rides::Structure::RideDayGroup> examplesA;
			std::vector<DataProcessing::Rides::Structure::RideDayGroup> examplesB;
			std::vector<DataProcessing::Rides::Structure::RideDayGroup> examplesC;
			std::vector<DataProcessing::Rides::Structure::RideDayGroup> examplesD;

			{
				examplesA.push_back(example);
			}

			{
				for (auto& otherExample : examples)
					if (otherExample.day != example.day)
						examplesB.push_back(otherExample);
			}

			{
				auto nn = Prediction::NeuralNetwork::NeuralNetworkReader::read("../../Resources/learning/nn/" + DataProcessing::Rides::Structure::Day::to_string(example.day));

				double minPrediction = std::numeric_limits<double>::max();
				size_t minPredictionIndex = -1;

				for (size_t i = 0; i < examples.size(); i++)
				{
					if (example.day == examples[i].day)
						continue;

					std::vector<double> values;
					for (auto value : features[DataProcessing::Rides::Structure::Day::to_string(example.day)])
						values.push_back(value);
					for (auto value : features[DataProcessing::Rides::Structure::Day::to_string(examples[i].day)])
						values.push_back(value);

					double preditedValue = nn.predict(values);

					if (preditedValue < minPrediction)
					{
						minPrediction = preditedValue;
						minPredictionIndex = i;
					}
				}

				examplesC.push_back(examples[minPredictionIndex]);
			} 
			
			{
				examplesD.push_back(examples[std::rand() % examples.size()]);
			}

			auto modelA = testPredictionPrepareModel(examplesA, stations, timeFrames, initialStationSize);
			auto modelB = testPredictionPrepareModel(examplesB, stations, timeFrames, initialStationSize);
			auto modelC = testPredictionPrepareModel(examplesC, stations, timeFrames, initialStationSize);
			auto modelD = testPredictionPrepareModel(examplesD, stations, timeFrames, initialStationSize);

			std::cout 
				<< DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelB) << " " 
				<< DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelC) << " "
				<< DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelD) << std::endl;
		}
	}
}