#pragma once

#include "../day-distance-function.hpp"

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

namespace CityBikes::Data::DayDistance::Utils
{
	class DayDistanceFunctionReader
	{
	public:
		static DayDistanceFunction readData(std::experimental::filesystem::path path)
		{
			DayDistanceFunction distanceFunction;

			std::ifstream file(path);

			size_t functionsNumber;
			file >> functionsNumber;

			for (size_t i = 0; i < functionsNumber; i++)
			{
				std::tm functionDay;
				file >> std::get_time(&functionDay, "%d.%m.%Y");

				size_t daysNumber;
				file >> daysNumber;

				for (size_t j = 0; j < daysNumber; j++)
				{
					std::tm distanceDay;
					file >> std::get_time(&distanceDay, "%d.%m.%Y");

					float distance;
					file >> distance;

					distanceFunction[functionDay][distanceDay] = distance;
				}
			}

			file.close();

			return distanceFunction;
		}		
		
		static void writeData(std::experimental::filesystem::path path, const DayDistanceFunction& distanceFunction)
		{
			std::ofstream file(path);

			file << distanceFunction.size() << std::endl;

			for (const auto& source : distanceFunction)
			{
				file << Time::Day::to_string(source.first) << " ";
				file << source.second.size() << std::endl;

				for (const auto& destination : source.second)
				{
					file << Time::Day::to_string(destination.first) << " ";
					file << destination.second << std::endl;
				}
			}

			file.close();
		}
	};
}