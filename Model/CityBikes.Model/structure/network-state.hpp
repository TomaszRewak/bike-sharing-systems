#pragma once

#include <vector>

#include "network-node.hpp"

namespace CityBikes::Model::Structure
{
	struct NetworkState
	{
		std::vector<NetworkNode> nodes;

		NetworkState(size_t nodes) :
			nodes(nodes)
		{ }
	};
}