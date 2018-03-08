#pragma once

#include <vector>

#include "network-filling-matrix.hpp"

namespace CityBikes::Flow::Filling
{
	/// <summary> Changes the fillness of nodes for all time frames simultaneously without a need for a recalculation </summary>
	class NetworkFillingMatrixAlteration
	{
	public:
		const size_t nodes;

	private:
		NetworkFillingMatrix & matrix;
		std::vector<int> nodeAlterations;

	public:
		NetworkFillingMatrixAlteration(NetworkFillingMatrix & matrix) :
			matrix(matrix),
			nodeAlterations(matrix.nodes, 0),
			nodes(matrix.nodes)
		{ }

		int& operator[](size_t node)
		{
			return nodeAlterations[node];
		}

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
	};
}