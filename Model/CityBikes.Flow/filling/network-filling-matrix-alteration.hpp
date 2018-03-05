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
		NetworkFillingMatrixAlteration(NetworkFillingMatrix & matrix, size_t nodes) :
			matrix(matrix),
			nodeAlterations(nodes)
		{ }

		void alter(size_t node, int by)
		{
			nodeAlterations[node] += by;
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
	};
}