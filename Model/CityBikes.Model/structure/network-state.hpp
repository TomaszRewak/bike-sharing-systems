#pragma once

#include <array>

#include "network-node.hpp"

namespace CityBikes::Model::Structure
{
	template<size_t Nodes>
	struct NetworkState
	{
		std::array<NetworkNode, Nodes> nodes;
	};
}