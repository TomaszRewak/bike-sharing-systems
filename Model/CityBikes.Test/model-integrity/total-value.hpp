#pragma once

#include "../../CityBikes.Model/structure/network-state.hpp"

namespace CityBikes::Test::ModelIntegrity
{
	double computeTotalNumber(Model::Structure::NetworkState& state)
	{
		double totalValue = 0;

		for (auto& node : state.nodes)
			totalValue += std::round(node.value);

		return totalValue;
	}
}