#pragma once

#include "structure/network-state.hpp"

#include <queue>

namespace CityBikes::Model
{
	template<size_t Nodes>
	struct FlowDistributionModel
	{
		std::vector<Structure::NetworkState<Nodes>> timeFrames;

		/// <summary> Alters the state of the network begging a the given time frame. "Forces" all rents (can lead to negative load). </summary>
		void alter(size_t timeFrame, size_t node, double value)
		{
			for (; timeFrame < timeFrames.size(); timeFrame++)
				timeFrames[timeFrame].nodes[node].value += value;
		}

		FlowDistributionModel<Nodes> getPart(size_t skip) const
		{
			FlowDistributionModel<Nodes> model;

			for (size_t i = skip; i < timeFrames.size(); i++)
				model.timeFrames.push_back(timeFrames[i]);

			return model;
		}
	};
}