#pragma once

#include "relocation-scheduling-configuration.hpp"
#include "scheduling-configuration.hpp"

namespace CityBikes::Flow::Configuration
{
	struct SimulationConfiguration
	{
		RelocationSchedulingConfiguration relocationSchedulingConfiguration;
		SchedulingConfiguration schedulingConfiguration;

		Model::Configuration::FlowDistributionModelSimulationConfiguration realFlowSimulationConfiguration;
		Model::Configuration::FlowDistributionModelSimulationConfiguration predictedFlowSimulationConfiguration;

		SimulationConfiguration(
			RelocationSchedulingConfiguration relocationSchedulingConfiguration,
			SchedulingConfiguration schedulingConfiguration,
			Model::Configuration::FlowDistributionModelSimulationConfiguration realFlowSimulationConfiguration,
			Model::Configuration::FlowDistributionModelSimulationConfiguration predictedFlowSimulationConfiguration
		):
			relocationSchedulingConfiguration(relocationSchedulingConfiguration),
			schedulingConfiguration(schedulingConfiguration),
			realFlowSimulationConfiguration(realFlowSimulationConfiguration),
			predictedFlowSimulationConfiguration(predictedFlowSimulationConfiguration)
		{ }
	};



}