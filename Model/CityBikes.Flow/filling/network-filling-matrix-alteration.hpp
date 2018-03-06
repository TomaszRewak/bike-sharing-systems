#pragma once

#include <vector>

#include "network-filling-matrix.hpp"

namespace CityBikes::Flow::Filling
{
	class NetworkFillingMatrixAlteration
	{
	private:
		NetworkFillingMatrix & matrix;
		std::vector<int> nodeAlterations;

	public:
		NetworkFillingMatrixAlteration(NetworkFillingMatrix & matrix) :
			matrix(matrix),
			nodeAlterations(matrix.nodes())
		{ }

		void alter(size_t node, int by)
		{
			nodeAlterations[node] += by;
		}

		int getAlteration(size_t node)
		{
			return nodeAlterations[node];
		}

		void setAlteration(size_t node, int value)
		{
			nodeAlterations[node] = value;
;		}

		int getNumber(size_t timeFrame, size_t node)
		{
			return matrix.getNumber(timeFrame, node) + nodeAlterations[node];
		}

		size_t getAboveThreshold(size_t timeFrame, size_t node, int threshold)
		{
			threshold -= nodeAlterations[node];

			return matrix.getAboveThreshold(timeFrame, node, threshold);
		}

		size_t getBelowThreshold(size_t timeFrame, size_t node, int threshold)
		{
			threshold -= nodeAlterations[node];

			return matrix.getBelowThreshold(timeFrame, node, threshold);
		}

		Structure::NodeFillingDefinition getNodeFillingDefinition(size_t node)
		{
			auto nodeFillingDefinition = matrix.getNodeFillingDefinition(node);

			nodeFillingDefinition.minNumber += nodeAlterations[node];
			nodeFillingDefinition.maxNumber += nodeAlterations[node];

			return nodeFillingDefinition;
		}
	};
}