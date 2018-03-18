#pragma once

#include <vector>
#include <limits>
#include <algorithm>

namespace CityBikes::Data::FillLevelDistribution
{
	struct FillLevelDistributionPredictionNode
	{
	private:
		std::vector<size_t> distribution;

		int minValue;
		int maxValue;

	public:

		int baseValue;

		FillLevelDistributionPredictionNode() :
			minValue(std::numeric_limits<int>::max()),
			maxValue(std::numeric_limits<int>::min()),
			baseValue(0)
		{ }

		FillLevelDistributionPredictionNode(int baseValue) :
			minValue(baseValue),
			maxValue(baseValue),
			baseValue(baseValue)
		{
			distribution = std::vector<size_t>{ 1 };
		}

		FillLevelDistributionPredictionNode(int baseValue, const FillLevelDistributionPredictionNode& nextFrame) :
			minValue(std::min(baseValue, nextFrame.minValue)),
			maxValue(std::max(baseValue, nextFrame.maxValue)),
			baseValue(baseValue)
		{
			distribution = std::vector<size_t>(maxValue - minValue + 1);

			for (int value = minValue; value <= maxValue; value++)
				distribution[value - minValue] = nextFrame[value];

			distribution[baseValue - minValue]++;
		}

		size_t operator[](int value) const
		{
			if (value < minValue || value > maxValue)
				return 0;

			return distribution[value - minValue];
		}

		size_t getAboveThreshold(int threshold) const
		{
			threshold = std::max(minValue - 1, threshold);

			size_t count = 0;

			for (int value = threshold + 1; value <= maxValue; value++)
				count += distribution[value - minValue];

			return count;
		}

		size_t getBelowThreshold(int threshold) const
		{
			threshold = std::min(maxValue + 1, threshold);

			size_t count = 0;

			for (int value = threshold - 1; value >= minValue; value--)
				count += distribution[value - minValue];

			return count;
		}
	};
}