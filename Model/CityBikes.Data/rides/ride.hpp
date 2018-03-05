#pragma once

#include <ctime>

namespace CityBikes::Data::Rides
{
	struct Ride
	{
		std::tm startTime;
		std::tm endTime;

		size_t sourceStation;
		size_t destinationStation;
	};
}