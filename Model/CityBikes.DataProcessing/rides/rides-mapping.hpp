#pragma once

#include "../../CityBikes.Data/rides/ride.hpp"
#include "../common/time-processing.hpp"
#include "../../CityBikes.Data/common/day.hpp"
#include "../../CityBikes.Model/data/flow-instance.hpp"

#include <vector>
#include <map>

namespace CityBikes::DataProcessing::Rides
{
	class RidesMapper
	{
	public:
		static std::map<Data::Common::Day, std::vector<Model::Data::FlowInstance>> map(std::vector<Data::Rides::Ride>& rides, size_t timeFrames)
		{
			std::map<Data::Common::Day, std::vector<Model::Data::FlowInstance>> groupedInstances;
			Common::TimeQuantizer timeQuantizer(timeFrames);

			for (auto& ride : rides)
			{
				Model::Data::FlowInstance flow;

				flow.source.node = ride.sourceStation;
				flow.source.timeFrame = timeQuantizer.quantize(ride.startTime);

				flow.destination.node = ride.destinationStation;
				flow.destination.timeFrame = timeQuantizer.quantize(ride.endTime);

				if (ride.startTime.tm_yday != ride.endTime.tm_yday)
					flow.destination.timeFrame = timeFrames;

				//flow.source.timeFrame = 2;
				//flow.destination.timeFrame = 10;

				groupedInstances[ride.startTime].push_back(flow);
			}

			return groupedInstances;
		}
	};
}