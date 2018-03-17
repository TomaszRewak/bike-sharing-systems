#pragma once

#include "time-processing.hpp"

#include <vector>
#include <algorithm>

namespace CityBikes::DataProcessing::Time
{
	class TimeSeriesQuantizer
	{
	private:
		size_t timeFrames;

	public:
		TimeSeriesQuantizer(size_t timeFrames) :
			timeFrames(timeFrames)
		{ }

		std::vector<double> quantize(std::vector<std::pair<std::tm, double>> limits) const
		{
			TimeQuantizer quantizer(timeFrames);

			std::sort(
				limits.begin(),
				limits.end(),
				[&](const auto& a, const auto& b)
			{
				return quantizer.quantize(a.first) < quantizer.quantize(b.first);
			});

			std::vector<double> values(timeFrames);

			size_t limitA = 0;
			size_t limitB = 0;

			for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
			{
				while (limitA < limits.size() - 1 && quantizer.quantize(limits[limitA + 1].first) < timeFrame)
					limitA++;

				while (limitB < limits.size() - 1 && quantizer.quantize(limits[limitB].first) < timeFrame)
					limitB++;

				double timeA = quantizer.quantize(limits[limitA].first);
				double timeB = quantizer.quantize(limits[limitB].first);

				double valueA = limits[limitA].second;
				double valueB = limits[limitB].second;

				double factor = (timeFrame - timeA) / (timeB - timeA);
				factor = std::max(0., std::min(1., factor));

				values[timeFrame] = valueA + factor * (valueB - valueA);
			}

			return values;
		}
	};
}