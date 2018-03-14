#pragma once

#include <vector>

#include "network-filling-matrix.hpp"

namespace CityBikes::Flow::Filling
{
	/// <summary> Changes the fillness of nodes for all time frames simultaneously without a need for a recalculation </summary>
	template<size_t Nodes>
	class NetworkFillingMatrixAlteration
	{
	private:
		const NetworkFillingMatrix<Nodes> & matrix;
		std::array<int, Nodes> nodeAlterations{};

	public:
		NetworkFillingMatrixAlteration(const NetworkFillingMatrix<Nodes> & matrix) :
			matrix(matrix)
		{ }

		int& operator[](size_t node)
		{
			return nodeAlterations[node];
		}

		int getNumber(size_t timeFrame, size_t node) const
		{
			return matrix.getNumber(timeFrame, node) + nodeAlterations[node];
		}

		size_t getAboveThreshold(size_t timeFrame, size_t node, int threshold) const
		{
			threshold -= nodeAlterations[node];

			return matrix.getAboveThreshold(timeFrame, node, threshold);
		}

		size_t getBelowThreshold(size_t timeFrame, size_t node, int threshold) const
		{
			threshold -= nodeAlterations[node];

			return matrix.getBelowThreshold(timeFrame, node, threshold);
		}
	};
}