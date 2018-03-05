#pragma once

#include "structure/network-state.hpp"

#include <queue>

namespace CityBikes::Model
{
	struct FlowDistributionModel
	{
		size_t startFrame;
		std::vector<Structure::NetworkState> timeFrames;

		FlowDistributionModel(size_t timeFrames, size_t nodes) :
			timeFrames(timeFrames, Structure::NetworkState(nodes))
		{ }
	};
}