#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "../demand-prediction.hpp"
#include "../../../CityBikes.DataProcessing/time/time-series-processing.hpp"
#include "../../common/day.hpp"

namespace CityBikes::Data::Demand::Utils
{
	template<size_t Nodes>
	class FlowTimeMatricesReader
	{
	public:
		static std::map<Common::Day, DemandPrediction<Nodes>> readData(std::experimental::filesystem::path path, size_t timeFrames)
		{
			std::map<Common::Day, DemandPrediction<Nodes>> result;

			DataProcessing::Time::TimeSeriesQuantizer timeSeriesQuantizer(timeFrames);

			std::ifstream file(path);

			size_t examples;
			size_t nodes;

			file >> examples >> nodes;

			for (size_t example = 0; example < examples; example++)
			{
				DemandPrediction<Nodes> demand(timeFrames);

				std::tm day;
				size_t timeStamps;

				file >> std::get_time(&day, "%d.%m.%Y") >> timeStamps;

				std::array<std::vector<std::pair<std::tm, double>>, Nodes> baseValues;

				for (size_t timeStamp = 0; timeStamp < timeStamps; timeStamp++)
				{
					std::tm time;
					file >> std::get_time(&time, "%H:%M");

					for (size_t node = 0; node < nodes; node++)
					{
						double value;
						file >> value;

						baseValues[node].push_back(std::make_pair(time, value));
					}
				}

				for (size_t node = 0; node < nodes; node++)
				{
					std::vector<double> quantizedValues = timeSeriesQuantizer.quantize(baseValues[node]);

					for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
						demand[timeFrame][node] = quantizedValues[timeFrame];
				}

				result.emplace(day, demand);
			}

			file.close();

			return result;
		}

		static void writeData(
			std::experimental::filesystem::path path,
			const std::map<Common::Day, DemandPrediction<Nodes>>& examples,
			size_t timeStamps
		)
		{
			std::ofstream file(path);

			file << examples.size() << " " << Nodes << std::endl;

			for (const auto& example : examples)
			{
				size_t timeFrames = example.second.size();

				DataProcessing::Time::TimeQuantizer timeQuantizer(timeFrames);

				file << Common::Day::to_string(example.first) << " " << timeStamps << std::endl;

				for (size_t timeStamp = 0; timeStamp < timeStamps; timeStamp++)
				{
					size_t timeFrame = timeFrames * timeStamp / timeStamps;

					std::tm time = timeQuantizer.dequantize(timeFrame);
					file << std::setw(2) << std::setfill('0') << time.tm_hour;
					file << ":";
					file << std::setw(2) << std::setfill('0') << time.tm_min;
					file << "\t";

					for (double node : example.second[timeFrame])
						file << node << "\t";

					file << std::endl;
				}
			}

			file.close();
		}
	};
}