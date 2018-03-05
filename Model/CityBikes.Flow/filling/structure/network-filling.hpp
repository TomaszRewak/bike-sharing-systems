#pragma once

#include "node-filling.hpp"

namespace CityBikes::Flow::Filling::Structure
{
	class NetworkFilling
	{
	private:
		std::vector<NodeFilling> nodes;

	public:
		NetworkFilling(std::vector<NodeFillingDefinition>& nodeDefinitons)
		{ 
			for (auto& nodeDefinition : nodeDefinitons)
				nodes.push_back(nodeDefinition);
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