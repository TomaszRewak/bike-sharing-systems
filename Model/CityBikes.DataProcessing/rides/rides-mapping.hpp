#pragma once

#include "structure/day.hpp"
#include "structure/ride-day-group.hpp"
#include "../../CityBikes.Data/rides/ride.hpp"
#include "../common/time-processing.hpp"

#include <vector>
#include <map>

namespace CityBikes::DataProcessing::Rides
{
	class RidesMapper
	{
	public:
		static std::vector<Structure::RideDayGroup> map(std::vector<Data::Rides::Ride>& rides, size_t timeFrames)
		{
			std::map<Structure::Day, std::vector<Model::Data::FlowInstance>> groupedInstances;
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

			std::vector<Structure::RideDayGroup> result;

			for (auto const&[day, flows] : groupedInstances)
			{
				tm time = day;
				Structure::RideDayGroup group(day);

				group.flowInstances = flows;
				group.features.dayOfTheWeek = time.tm_wday;

				result.push_back(group);
			}

			return result;
		}
	};
}