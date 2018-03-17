#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.StaticAnalysis/static-demand-analysis.hpp"
#include "../../CityBikes.DataProcessing/validation/availability-validation.hpp"

#include <iostream>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testStaticAnalysis(size_t timeFrames, size_t examplesNumber)
	{
		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		std::vector<Model::Data::FlowAction> actions;
		for (size_t i = 0; i < examplesNumber; i++)
		{
			auto example = examples.begin();
			std::advance(example, std::rand() % examples.size());

			for (auto& flowInstance : example->second)
				actions.push_back(Model::Data::FlowAction(flowInstance, 1. / examplesNumber));
		}

		std::cout << "Simulating" << std::endl;

		StaticAnalysis::Configuration::DemandAnalysisFlowConfiguration configuration(actions);
		auto demand = StaticAnalysis::StaticDemandAnalysis<Nodes>::analyse(configuration);

		Model::Structure::NetworkState<Nodes> initialState;
		for (size_t node = 0; node < Nodes; node++)
			initialState.nodes[node].value = demand[node] + 1;

		Model::Configuration::FlowDistributionModelSimulationConfiguration simulationConfiguration(actions);
		Model::FlowDistributionModelSimulation<Nodes> simulation(
			simulationConfiguration,
			initialState,
			false);

		auto model = simulation.run(timeFrames);

		std::cout << DataProcessing::Validation::AvailabilityValidation::getTimeOfUnavailability(model);

		std::getchar();
	}
}