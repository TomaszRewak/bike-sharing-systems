#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>

#include "../ride.hpp"

namespace CityBikes::Data::Rides::Utils
{
	class RideReader
	{
	public:
		static std::vector<Ride> readData(std::experimental::filesystem::path path)
		{
			std::vector<Ride> result;
			std::ifstream file(path);

			size_t examples;
			file >> examples;

			for (size_t i = 0; i < examples; i++)
			{
				Ride ride;

				file >> std::get_time(&ride.startTime, "%d.%m.%Y %H:%M");
				file >> std::get_time(&ride.endTime, "%d.%m.%Y %H:%M");

				std::mktime(&ride.startTime);
				std::mktime(&ride.endTime);
				
				file >> ride.sourceStation;
				file >> ride.destinationStation;

				result.push_back(ride);
			}

			file.close();
			return result;
		}
	};
}