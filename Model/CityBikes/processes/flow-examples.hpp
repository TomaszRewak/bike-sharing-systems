#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"

namespace CityBikes::Processes
{
	void computeFlowExamples(size_t stations, size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		for (auto &day : examples)
		{
			Model::Structure::NetworkState initialState(stations);

			std::vector<Model::Data::FlowAction> actions;
			for (auto& flowInstance : day.flowInstances)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1.));

			Model::FlowDistributionModelSimulation simulation(actions);

			auto model = simulation.simulate(initialState, 0, timeFrames, true);

			auto fileName = DataProcessing::Rides::Structure::Day::to_string(day.day);
			Data::Predictions::Utils::PredictionWriter::writeData(model, "../../Resources/processed/base_predictions/" + fileName);
		}

		std::getchar();
	}
}