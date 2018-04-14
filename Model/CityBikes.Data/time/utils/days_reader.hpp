#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <set>

#include "../day.hpp"

namespace CityBikes::Data::Time::Utils
{
	class DaysReader
	{
	public:
		static std::set<Time::Day> readData(std::experimental::filesystem::path path)
		{
			std::set<Time::Day> result;

			std::ifstream file(path);

			size_t examples;
			file >> examples;

			for (size_t example = 0; example < examples; example++)
			{
				std::tm day;
				file >> std::get_time(&day, "%d.%m.%Y");

				result.insert(day);
			}

			file.close();

			return result;
		}
	};
}