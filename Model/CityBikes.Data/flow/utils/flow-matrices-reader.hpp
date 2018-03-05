#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "../flow-matrix.hpp"
#include "../../../CityBikes.DataProcessing/common/time-processing.hpp"

namespace CityBikes::Data::Flow::Utils
{
	class FlowMatricesReader
	{
	public:
		static std::map<std::string, FlowMatrix> readData(std::experimental::filesystem::path path, size_t stations, size_t timeFrames)
		{
			std::map<std::string, FlowMatrix> result;
			DataProcessing::Common::TimeQuantizer timeQuantizer(timeFrames);

			std::ifstream file(path);

			size_t matricesNumber;
			file >> matricesNumber;

			for (size_t m = 0; m < matricesNumber; m++)
			{
				FlowMatrix matrix(stations);

				std::string matrixName;
				file >> matrixName;

				int stationsNumber;
				file >> stationsNumber;

				for (size_t s = 0; s < stationsNumber; s++)
				{
					size_t station;
					file >> station;

					int connectionsNumber;
					file >> connectionsNumber;

					for (size_t c = 0; c < connectionsNumber; c++)
					{
						size_t connectedStation;
						file >> connectedStation;

						int constraintsNumber;
						file >> constraintsNumber;

						for (size_t i = 0; i < constraintsNumber; i++)
						{
							FlowConstraint constraint;

							std::tm startTime;
							file >> std::get_time(&startTime, "%H:%M");
							constraint.timeFrame = timeQuantizer.quantize(startTime);

							size_t flowDuration;
							file >> flowDuration;
							constraint.flowDuration = std::max(1, timeQuantizer.quantize(flowDuration));

							matrix.addConstraint(station, connectedStation, constraint);
						}
					}
				}

				for (size_t s = 0; s < stationsNumber; s++)
				{
					FlowConstraint constraint;

					constraint.flowDuration = 1;
					constraint.timeFrame = 1;

					matrix.addConstraint(s, s, constraint);
				}

				result.emplace(matrixName, matrix);
			}

			file.close();

			return result;
		}
	};
}