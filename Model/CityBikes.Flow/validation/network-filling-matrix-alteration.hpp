#pragma once

#include <vector>

namespace CityBikes::Flow::Validation
{
	class NetworkFillingMatrixAlteration
	{
	private:
		std::vector<int> nodeAlterations;

	public:
		NetworkFillingMatrixAlteration(size_t nodes) :
			nodeAlterations(nodes)
		{ }

		void alter(size_t node, int by)
		{
			nodeAlterations[node] += by;
		}

		int getAlteration(size_t node)
		{
			return nodeAlterations[node];
		}
	};
}