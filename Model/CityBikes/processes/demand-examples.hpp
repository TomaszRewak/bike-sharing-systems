#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/demand/utils/demand-prediction-reader.hpp"
#include "../../CityBikes.Model/prediction/demand-analysis.hpp"

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void computeDemandExamples()
	{
		size_t timeFrames = 1440;

		auto rides = Data::Rides::Utils::RideReader::readData("../../Resources/processed/rides.rides");
		auto examples = DataProcessing::Rides::RidesMapper::map(rides, timeFrames);

		std::map<Data::Common::Day, Data::Demand::DemandPrediction<Nodes>> demands;

		for (auto &day : examples)
		{
			Model::Structure::NetworkState<Nodes> initialState;

			std::vector<Data::Flow::FlowAction> actions;
			for (auto& flowInstance : day.second)
				actions.push_back(Data::Flow::FlowAction(flowInstance, 1.));

			Data::Demand::DemandPrediction<Nodes> demand = Model::Prediction::DemandAnalysis<Nodes>::computeDemand(actions, timeFrames);

			demands.emplace(day.first, demand);
		}

		Data::Demand::Utils::FlowTimeMatricesReader::writeData<Nodes>(
			"../../Resources/processed/examples_demand.demand",
			demands,
			144);

		std::getchar();
	}
}