#pragma once

#include <ctime>

namespace CityBikes::DataProcessing::Common
{
	class TimeQuantizer
	{
	private:
		size_t timeFrames;

	public:
		TimeQuantizer(size_t timeFrames) :
			timeFrames(timeFrames)
		{ }

		size_t quantize(std::tm time) const
		{
			return timeFrames * (time.tm_hour * 60 + time.tm_min) / 1440;
		}

		size_t quantize(size_t seconds) const
		{
			return timeFrames * seconds / 86400;
		}
	};
}