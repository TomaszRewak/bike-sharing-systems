#pragma once

#include "node-filling.hpp"

#include <array>

namespace CityBikes::Flow::Filling::Structure
{
	template<size_t Nodes>
	class NetworkFilling
	{
	private:
		std::array<NodeFilling, Nodes> nodes;

	public:
		NetworkFilling(std::array<NodeFillingDefinition, Nodes>& nodeDefinitons)
		{ 
			for (size_t node = 0; node < Nodes; node++)
				nodes[node] = nodeDefinitons[node];
		}

		size_t getFilling(size_t node, int number)
		{
			return nodes[node].getFilling(number);
		}

		void setFilling(size_t node, int number, size_t filling)
		{
			nodes[node].setFilling(number, filling);
		}
	};
}