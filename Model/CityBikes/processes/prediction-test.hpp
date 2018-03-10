#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.DataProcessing/prediction/prediction-offset.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Data/distance/utils/distance-functions-reader.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	Model::FlowDistributionModel<Nodes> testPredictionPrepareModel(std::vector<std::vector<Model::Data::FlowInstance>> examples, size_t timeFrames, size_t initialStationSize)
	{
		Model::Structure::NetworkState<Nodes> initialState;
		for (auto& station : initialState.nodes)
			station.value = initialStationSize;

		std::vector<Model::Data::FlowAction> actions;
		for (auto& example : examples)
			for (auto& flowInstance : example)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1. / examples.size()));

		Model::Configuration::FlowDistributionModelSimulationConfiguration simulationConfiguration(actions);
		Model::FlowDistributionModelSimulation<Nodes> simulation(
			simulationConfiguration,
			initialState,
			false);

		simulation.runTo(timeFrames);

		return simulation.getModel();
	}

	template<size_t Nodes>
	void testPrediction(size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto distanceFunctions = Data::Distance::Utils::DistanceFunctionsReader::readData("../../Resources/learning/nn.model");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		float comparedToAll = 0;
		float comparedToEmpty = 0;

		for (auto& example : examples)
		{
			std::vector<std::vector<Model::Data::FlowInstance>> examplesA;
			std::vector<std::vector<Model::Data::FlowInstance>> examplesB;
			std::vector<std::vector<Model::Data::FlowInstance>> examplesC;
			std::vector<std::vector<Model::Data::FlowInstance>> examplesD;
			std::vector<std::vector<Model::Data::FlowInstance>> examplesE;

			{
				examplesA.push_back(example.second);
			}

			{
				for (auto& otherExample : examples)
					if (otherExample.first != example.first)
						examplesB.push_back(otherExample.second);
			}

			{
				std::vector<std::pair<float, Data::Common::Day>> sortedDays;

				for (auto otherExample : examples)
					if (otherExample.first != example.first)
						sortedDays.push_back(std::make_pair(distanceFunctions[example.first][otherExample.first], otherExample.first));

				std::sort(
					sortedDays.begin(),
					sortedDays.end(),
					[](auto a, auto b) { return a.first < b.first; }
				);

				for (size_t i = 0; i < 100; i++)
				{
					examplesC.push_back(examples[sortedDays[i].second]);
				}
			}

			{
				auto example = examples.begin();
				std::advance(example, std::rand() % examples.size());

				examplesD.push_back(example->second);
			}

			{

			}

			auto modelA = testPredictionPrepareModel<Nodes>(examplesA, timeFrames, initialStationSize); // base
			auto modelB = testPredictionPrepareModel<Nodes>(examplesB, timeFrames, initialStationSize); // all
			auto modelC = testPredictionPrepareModel<Nodes>(examplesC, timeFrames, initialStationSize); // selected
			auto modelD = testPredictionPrepareModel<Nodes>(examplesD, timeFrames, initialStationSize); // random
			auto modelE = testPredictionPrepareModel<Nodes>(examplesE, timeFrames, initialStationSize); // empty

			float diffBase = DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelA);
			float diffAll = DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelB);
			float diffSelected = DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelC);
			float diffRandom = DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelD);
			float diffEmpty = DataProcessing::Prediction::PredictionOffset::computeOffset(modelA, modelE);

			comparedToAll += diffAll - diffSelected;
			comparedToEmpty += diffEmpty - diffSelected;

			std::cout << diffBase << " " <<  diffAll << " " << diffSelected << " " << diffRandom << " " << diffEmpty << " | " << comparedToAll << " " << comparedToEmpty << std::endl;
		}

		std::getchar();
	}
}