#pragma once

#include "../../CityBikes.Data/rides/utils/ride-reader.hpp"
#include "../../CityBikes.DataProcessing/rides/rides-mapping.hpp"
#include "../../CityBikes.Data/flow-time/utils/flow-time-matrices-reader.hpp"

#include <iostream>
#include <limits>

namespace CityBikes::Processes
{
	template<size_t Nodes>
	void testFlow(size_t timeFrames, size_t initialStationSize, size_t examplesNumber)
	{
		auto matrices = Data::FlowTime::Utils::FlowTimeMatricesReader::readData<Nodes>("../../Resources/processed/time_matrices.time", timeFrames);

		size_t source = 0;// std::rand() % Nodes;
		size_t destination = 2;// std::rand() % Nodes;

		std::cout << source << " " << destination << std::endl;

		for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
			std::cout
			<< matrices.begin()->second.flowDuration(source, destination, timeFrame) << " "
			<< matrices.rbegin()->second.flowDuration(source, destination, timeFrame) << std::endl;

		std::getchar();
	}
}