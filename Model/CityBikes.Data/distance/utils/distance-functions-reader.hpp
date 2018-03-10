#pragma once

#include "../distance-function.hpp"

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

namespace CityBikes::Data::Distance::Utils
{
	class DistanceFunctionsReader
	{
	public:
		static std::map<Common::Day, DistanceFunction> readData(std::experimental::filesystem::path path)
		{
			std::map<Common::Day, DistanceFunction> distanceFunctions;

			std::ifstream file(path);

			size_t functionsNumber;
			file >> functionsNumber;

			for (size_t i = 0; i < functionsNumber; i++)
			{
				DistanceFunction distanceFunction;

				std::tm functionDay;
				file >> std::get_time(&functionDay, "%Y-%m-%d");

				size_t daysNumber;
				file >> daysNumber;

				for (size_t j = 0; j < daysNumber; j++)
				{
					std::tm distanceDay;
					file >> std::get_time(&distanceDay, "%Y-%m-%d");

					float distance;
					file >> distance;

					distanceFunction[distanceDay] = distance;
				}

				distanceFunctions[functionDay] = distanceFunction;
			}

			file.close();

			return distanceFunctions;
		}
	};
}