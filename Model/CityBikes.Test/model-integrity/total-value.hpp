#pragma once

#include "../../CityBikes.Model/structure/network-state.hpp"

namespace CityBikes::Test::ModelIntegrity
{
	template<size_t Nodes>
	double computeTotalNumber(Model::Structure::NetworkState<Nodes>& state)
	{
		double totalValue = 0;

		for (auto& node : state.nodes)
			totalValue += std::round(node.value);

		return totalValue;
	}
}