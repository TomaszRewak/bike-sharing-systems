#pragma once

#include "../day-distance-function.hpp"

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

namespace CityBikes::Data::DayDistance::Utils
{
	template<size_t Nodes>
	class DayDistanceFunctionReader
	{
	public:
		static DayDistanceFunction<Nodes> readData(std::experimental::filesystem::path path)
		{
			DayDistanceFunction<Nodes> distanceFunction;

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

					size_t nodes;
					file >> nodes;

					for (size_t node = 0; node < Nodes; node++)
					{
						double distance;
						file >> distance;

						distanceFunction[functionDay][distanceDay][node] = std::max(distance, 0.);
					}
				}
			}

			file.close();

			return distanceFunction;
		}		
		
		static void writeData(std::experimental::filesystem::path path, DayDistanceFunction<Nodes>& distanceFunction)
		{
			std::ofstream file(path);

			file << distanceFunction.size() << std::endl;
			
			for (const auto& source : distanceFunction)
			{
				file << Time::Day::to_string(source.first) << " ";
				file << source.second.size() << std::endl;

				for (const auto& destination : source.second)
				{
					file << Time::Day::to_string(destination.first) << " " << Nodes << std::endl;

					for (size_t node = 0; node < Nodes; node++)
						file << destination.second[node] << " ";

					file << std::endl;
				}
			}

			file.close();
		}
	};
}