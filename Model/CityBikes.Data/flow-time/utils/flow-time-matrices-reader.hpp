#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "../flow-time-prediction.hpp"
#include "../../../CityBikes.DataProcessing/time/time-series-processing.hpp"
#include "../../time/day.hpp"

namespace CityBikes::Data::FlowTime::Utils
{
	class FlowTimeMatricesReader
	{
	public:
		template<size_t Nodes>
		static std::map<Time::Day, FlowTimePrediction<Nodes>> readData(std::experimental::filesystem::path path, size_t timeFrames)
		{
			std::map<Time::Day, FlowTimePrediction<Nodes>> result;

			DataProcessing::Time::TimeQuantizer timeQuantizer(timeFrames);
			DataProcessing::Time::TimeSeriesQuantizer timeSeriesQuantizer(timeFrames);

			std::ifstream file(path);

			size_t matricesNumber;
			file >> matricesNumber;

			for (size_t m = 0; m < matricesNumber; m++)
			{
				FlowTimePrediction<Nodes> matrix(timeFrames);

				std::tm day;
				file >> std::get_time(&day, "%d.%m.%Y");

				size_t timeStamps;
				file >> timeStamps;

				std::array<std::array<std::vector<std::pair<std::tm, double>>, Nodes>, Nodes> baseValues;

				for (size_t timeStamp = 0; timeStamp < timeStamps; timeStamp++)
				{
					std::array<std::array<size_t, Nodes>, Nodes> baseFrame;

					std::tm time;
					file >> std::get_time(&time, "%H:%M");

					size_t stationsNumber;
					file >> stationsNumber;

					for (size_t source = 0; source < Nodes; source++)
					{
						for (size_t destination = 0; destination < Nodes; destination++)
						{
							double value;
							file >> value;

							baseValues[source][destination].push_back(std::make_pair(time, value));
						}
					}
				}

				for (size_t source = 0; source < Nodes; source++)
				{
					for (size_t destination = 0; destination < Nodes; destination++)
					{
						std::vector<double> quantizedValues = timeSeriesQuantizer.quantize(baseValues[source][destination]);

						for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
							matrix[timeFrame][source][destination] = timeQuantizer.quantize(quantizedValues[timeFrame]);
					}
				}

				result.emplace(day, matrix);
			}

			file.close();

			return result;
		}
	};
}