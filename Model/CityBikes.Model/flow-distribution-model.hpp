#pragma once

#include "structure/network-state.hpp"

#include <queue>

namespace CityBikes::Model
{
	struct FlowDistributionModel
	{
		const size_t nodes;

		std::vector<Structure::NetworkState> timeFrames;

		FlowDistributionModel(size_t timeFrames, size_t nodes) :
			timeFrames(timeFrames, Structure::NetworkState(nodes)),
			nodes(nodes)
		{ }

		/// <summary> Alters the state of the network begging a the given time frame. "Forces" all rents (can lead to negative load). </summary>
		void alter(size_t timeFrame, size_t node, double value)
		{
			for (; timeFrame < timeFrames.size(); timeFrame++)
				timeFrames[timeFrame].nodes[node].value += value;
		}
	};
}