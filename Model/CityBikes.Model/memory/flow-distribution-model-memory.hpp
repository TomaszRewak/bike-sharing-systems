#pragma once

#include <vector>

namespace CityBikes::Model::Memory
{
	struct FlowDistributionModelMemory
	{
		std::vector<std::vector<float>> timeFrames;

		FlowDistributionModelMemory(size_t timeFrames, size_t nodes):
			timeFrames(timeFrames, std::vector<float>(nodes, 0))
		{ }
	};
}