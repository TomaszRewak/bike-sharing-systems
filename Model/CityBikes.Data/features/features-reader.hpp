#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <map>

namespace CityBikes::Data::Features
{
	class FeaturesReader
	{
	public:
		static std::map<std::string, std::vector<double>> readData(std::experimental::filesystem::path path)
		{
			std::map<std::string, std::vector<double>> result;
			std::ifstream file(path);

			size_t examples, featuresNumber;
			file >> examples >> featuresNumber;

			for (size_t i = 0; i < examples; i++)
			{
				std::string name;
				file >> name;

				std::vector<double> features;

				for (size_t j = 0; j < featuresNumber; j++)
				{
					double value;
					file >> value;

					features.push_back(value);
				}

				result[name] = features;
			}

			file.close();
			return result;
		}
	};
}