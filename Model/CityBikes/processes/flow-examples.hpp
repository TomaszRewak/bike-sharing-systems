#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void computeFlowExamples(size_t timeFrames)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		for (auto &day : examples)
		{
			Model::Structure::NetworkState<Nodes> initialState;

			std::vector<Model::Data::FlowAction> actions;
			for (auto& flowInstance : day.flowInstances)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1.));

			Model::Configuration::FlowDistributionModelSimulationConfiguration simulationConfiguration(actions);
			Model::FlowDistributionModelSimulation<Nodes> simulation(
				simulationConfiguration,
				initialState,
				true);

			simulation.runTo(timeFrames);

			auto model = simulation.getModel();

			auto fileName = DataProcessing::Rides::Structure::Day::to_string(day.day);
			Data::Predictions::Utils::PredictionWriter::writeData(model, "../../Resources/processed/base_predictions/" + fileName);
		}

		std::getchar();
	}
}