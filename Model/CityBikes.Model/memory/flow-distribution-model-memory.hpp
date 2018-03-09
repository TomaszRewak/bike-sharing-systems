#pragma once

#include <vector>
#include <array>
#include <map>

namespace CityBikes::Model::Memory
{
	template<size_t Nodes>
	class FlowDistributionModelMemory
	{
	private:
		std::map<size_t, std::array<float, Nodes>> timeFrames;

	public:
		void add(size_t timeFrame, size_t node, float value)
		{
			timeFrames[timeFrame][node] += value;
		}

		std::array<float, Nodes> pop(size_t timeFrame)
		{
			std::array<float, Nodes> result = timeFrames[timeFrame];

			auto iterator = timeFrames.find(timeFrame);
			timeFrames.erase(iterator);

			return result;
		}
	};
}