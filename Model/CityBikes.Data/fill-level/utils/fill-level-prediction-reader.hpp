#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "../fill-level-prediction.hpp"
#include "../../time/day.hpp"

namespace CityBikes::Data::FillLevel::Utils
{
	class FlowTimeMatricesReader
	{
	public:
		template<size_t Nodes>
		static void writeData(
			std::experimental::filesystem::path path,
			const std::map<Time::Day, FillLevelPrediction<Nodes>>& examples)
		{
			std::ofstream file(path);

			file << examples.size() << " " << Nodes << std::endl;

			for (const auto& example : examples)
			{
				file << Time::Day::to_string(example.first) << " " << example.second.size() << std::endl;

				for (const FillLevelPredictionFrame<Nodes>& frame : example.second)
				{
					for (double node : frame)
						file << node << "\t";

					file << std::endl;
				}
			}

			file.close();
		}
	};
}