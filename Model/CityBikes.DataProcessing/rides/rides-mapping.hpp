#pragma once

#include "../../CityBikes.Data/rides/ride.hpp"
#include "../time/time-processing.hpp"
#include "../../CityBikes.Data/time/day.hpp"
#include "../../CityBikes.Data/flow/flow-instance.hpp"

#include <vector>
#include <map>
#include <algorithm>

namespace CityBikes::DataProcessing::Rides
{
	class RidesMapper
	{
	public:
		static std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> map(std::vector<Data::Rides::Ride>& rides, size_t timeFrames)
		{
			std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> groupedInstances;
			Time::TimeQuantizer timeQuantizer(timeFrames);

			for (auto& ride : rides)
			{
				Data::Flow::FlowInstance flow;

				flow.source.node = ride.sourceStation;
				flow.source.timeFrame = timeQuantizer.quantize(ride.startTime);

				flow.destination.node = ride.destinationStation;
				flow.destination.timeFrame = timeQuantizer.quantize(ride.endTime);

				if (ride.startTime.tm_yday != ride.endTime.tm_yday)
					flow.destination.timeFrame = timeFrames;

				flow.destination.timeFrame = std::max(flow.source.timeFrame, flow.destination.timeFrame);

				//flow.source.timeFrame = 2;
				//flow.destination.timeFrame = 10;

				groupedInstances[ride.startTime].push_back(flow);
			}

			return groupedInstances;
		}
	};
}