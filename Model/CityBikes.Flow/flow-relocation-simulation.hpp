#pragma once

#include "flow-relocation-model.hpp"
#include "../CityBikes.Model/flow-distribution-model-simulation.hpp"

namespace CityBikes::Flow
{
	class FlowRelocationSimulation
	{
	private:
		size_t predictionWindow;

	public:
		FlowRelocationSimulation(size_t predictionWindow) :
			predictionWindow(predictionWindow)
		{ }

		void simulate(
			size_t startFrame, size_t endFrame,
			Model::Structure::NetworkState initialState,
			Model::Configuration::FlowDistributionModelSimulationConfiguration& realFlowSimulationConfiguration,
			Model::Configuration::FlowDistributionModelSimulationConfiguration& predictedFlowSimulationConfiguration,
			FlowRelocationModel relocationModel,
			size_t timeFrames)
		{
			Model::FlowDistributionModelSimulation realFlowSimulation(
				realFlowSimulationConfiguration,
				initialState,
				timeFrames,
				false);

			Model::FlowDistributionModelSimulation predictedFlowSimulation(
				predictedFlowSimulationConfiguration,
				initialState,
				timeFrames + predictionWindow,
				true);

			for (size_t timeFrame = startFrame; timeFrame < endFrame; timeFrame++)
			{
				Model::FlowDistributionModelSimulation predictedFlowWindowSimulation = predictedFlowSimulation;
			}
		}
	};
}