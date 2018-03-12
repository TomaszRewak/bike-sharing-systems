#pragma once

#include <vector>

#include "node-filling-definition.hpp"

namespace CityBikes::Flow::Filling::Structure
{
	class NodeFilling
	{
	private:
		NodeFillingDefinition nodeFillingDefinition;

		std::vector<size_t> nodeFilling;

	public:
		NodeFilling()
		{ }

		NodeFilling(NodeFillingDefinition nodeFillingDefinition):
			nodeFillingDefinition(nodeFillingDefinition),
			nodeFilling(nodeFillingDefinition.maxNumber - nodeFillingDefinition.minNumber + 1, 0)
		{ }

		size_t getFilling(int number) const
		{
			if (number < nodeFillingDefinition.minNumber || number > nodeFillingDefinition.maxNumber)
				return 0;

			return nodeFilling[number - nodeFillingDefinition.minNumber];
		}

		void setFilling(int number, size_t filling)
		{
			if (number >= nodeFillingDefinition.minNumber && number <= nodeFillingDefinition.maxNumber)
				nodeFilling[number - nodeFillingDefinition.minNumber] = filling;
		}
	};
}