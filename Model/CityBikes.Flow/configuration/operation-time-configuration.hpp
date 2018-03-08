#pragma once

#include <algorithm>

namespace CityBikes::Flow::Configuration
{
	struct OperationTimeConfiguration
	{
	private:
		float baseOperationTime;
		float singleOperationTime;

	public:
		OperationTimeConfiguration(float baseOperationTime, float singleOperationTime) :
			baseOperationTime(baseOperationTime),
			singleOperationTime(singleOperationTime)
		{ }

		size_t getFillnessChangeTime(int fillChange) const
		{
			if (fillChange == 0)
				return 0;

			return std::round(
				baseOperationTime + std::abs(fillChange) * singleOperationTime
			);
		}
	};
}