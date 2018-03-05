#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.Model/flow-distribution-model-simulation.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.DataProcessing/prediction/prediction-offset.hpp"
#include "../../CityBikes.Data/predictions/utils/prediction-writer.hpp"
#include "../../CityBikes.Data/features/features-reader.hpp"
#include "../../CityBikes.Test/model-integrity/total-value.hpp"
#include "../../CityBikes.Data/flow/utils/flow-matrices-reader.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	void testFlow(size_t stations, size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto matrices = Data::Flow::Utils::FlowMatricesReader::readData("../../Resources/processed/flow_matrices.flow", stations, timeFrames);

		std::getchar();
	}
}