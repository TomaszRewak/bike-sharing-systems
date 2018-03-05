#pragma once

#include <vector>

#include "../../../CityBikes.Model/data/flow-instance.hpp"
#include "day.hpp"

namespace CityBikes::DataProcessing::Rides::Structure
{
	struct RideDayFeatures
	{
		int dayOfTheWeek;
	};

	struct RideDayGroup
	{
		RideDayGroup(Day day):
			day(day)
		{ }

		Day day;
		RideDayFeatures features;
		std::vector<Model::Data::FlowInstance> flowInstances;
	};
}