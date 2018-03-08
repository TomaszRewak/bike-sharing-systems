#pragma once

#include "structure/network-state.hpp"

#include <queue>

namespace CityBikes::Model
{
	struct FlowDistributionModel
	{
		const size_t nodes;

		std::vector<Structure::NetworkState> timeFrames;

		FlowDistributionModel(size_t nodes):
			nodes(nodes)
		{ }

		FlowDistributionModel(Structure::NetworkState initialState) :
			nodes(initialState.nodes.size())
		{ 
			timeFrames.push_back(initialState);
		}

		/// <summary> Alters the state of the network begging a the given time frame. "Forces" all rents (can lead to negative load). </summary>
		void alter(size_t timeFrame, size_t node, double value)
		{
			for (; timeFrame < timeFrames.size(); timeFrame++)
				timeFrames[timeFrame].nodes[node].value += value;
		}

		FlowDistributionModel getPart(size_t skip) const
		{
			FlowDistributionModel model(nodes);

			for (size_t i = skip; i < timeFrames.size(); i++)
				model.timeFrames.push_back(timeFrames[i]);
		}
	};
}