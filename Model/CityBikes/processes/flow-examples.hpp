#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"

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
			for (auto& flowInstance : day.second)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1.));

			Model::Configuration::FlowDistributionModelSimulationConfiguration simulationConfiguration(actions);
			Model::FlowDistributionModelSimulation<Nodes> simulation(
				simulationConfiguration,
				initialState,
				true);

			auto model = simulation.run(timeFrames);

			auto fileName = Data::Common::Day::to_string(day.first);
			Data::Predictions::Utils::PredictionWriter::writeData(model, "../../Resources/processed/base_predictions/" + fileName);
		}

		std::getchar();
	}
}