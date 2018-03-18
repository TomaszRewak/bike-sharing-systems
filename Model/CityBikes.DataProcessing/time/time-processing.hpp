#pragma once

#include <ctime>

namespace CityBikes::DataProcessing::Time
{
	class TimeQuantizer
	{
	private:
		size_t timeFrames;

	public:
		TimeQuantizer(size_t timeFrames) :
			timeFrames(timeFrames)
		{ }

		float quantize(std::tm time) const
		{
			return (double)timeFrames * (time.tm_hour * 60 + time.tm_min) / 1440;
		}

		float quantize(double seconds) const
		{
			return (double)timeFrames * seconds / 86400;
		}

		std::tm dequantize(double timeFrame)
		{
			std::tm time;

			time.tm_hour = timeFrame * 24 / timeFrames;
			time.tm_min = timeFrame * 1440 / timeFrames - time.tm_hour * 60;

			return time;
		}
	};
}