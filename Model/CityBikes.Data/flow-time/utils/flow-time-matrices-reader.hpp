#pragma once

#include <experimental/filesystem>
#include <fstream>
#include <iomanip>
#include <map>

#include "../flow-time-matrix.hpp"
#include "../../../CityBikes.DataProcessing/common/time-processing.hpp"
#include "../../common/day.hpp"

namespace CityBikes::Data::FlowTime::Utils
{
	class FlowTimeMatricesReader
	{
	public:
		template<size_t Nodes>
		static std::map<Common::Day, FlowTimeMatrix<Nodes>> readData(std::experimental::filesystem::path path, size_t timeFrames)
		{
			std::map<Common::Day, FlowTimeMatrix<Nodes>> result;
			DataProcessing::Common::TimeQuantizer timeQuantizer(timeFrames);

			std::ifstream file(path);

			size_t matricesNumber;
			file >> matricesNumber;

			for (size_t m = 0; m < matricesNumber; m++)
			{
				FlowTimeMatrix<Nodes> matrix;

				std::tm day;
				file >> std::get_time(&day, "%d.%m.%Y");

				size_t timeFramesNumber;
				file >> timeFramesNumber;

				std::vector<std::pair<size_t, std::array<std::array<size_t, Nodes>, Nodes>>> baseFrames;

				for (size_t timeFrame = 0; timeFrame < timeFramesNumber; timeFrame++)
				{
					std::array<std::array<size_t, Nodes>, Nodes> baseFrame;

					std::tm time;
					file >> std::get_time(&time, "%H:%M");

					size_t stationsNumber;
					file >> stationsNumber;

					for (size_t source = 0; source < Nodes; source++)
						for (size_t destination = 0; destination < Nodes; destination++)
							file >> baseFrame[source][destination];

					baseFrames.push_back(
						std::make_pair(
							timeQuantizer.quantize(time),
							baseFrame
						)
					);
				}

				for (size_t timeFrame = 0; timeFrame < timeFrames; timeFrame++)
				{
					Structure::FlowTimeMatrixFrame<Nodes> flowTimeFrame;

					size_t interpolationFrameA = 0;
					size_t interpolationFrameB = baseFrames.size() - 1;

					while (interpolationFrameA < interpolationFrameB && baseFrames[interpolationFrameA + 1].first <= timeFrame)
						interpolationFrameA++;
					while (interpolationFrameB > interpolationFrameA && baseFrames[interpolationFrameB - 1].first >= timeFrame)
						interpolationFrameB--;

					size_t timeFrameA = baseFrames[interpolationFrameA].first;
					size_t timeFrameB = baseFrames[interpolationFrameB].first;

					auto& matrixA = baseFrames[interpolationFrameA].second;
					auto& matrixB = baseFrames[interpolationFrameB].second;

					double interpolationFactor = (double)(timeFrame - timeFrameA) / (timeFrameB - timeFrameA);
					interpolationFactor = std::max(0., std::min(1., interpolationFactor));

					for (size_t source = 0; source < Nodes; source++)
						for (size_t destination = 0; destination < Nodes; destination++)
							flowTimeFrame[source][destination] = timeQuantizer.quantize(
								matrixA[source][destination] + interpolationFactor * (matrixB[source][destination] - matrixA[source][destination])
							);

					matrix.timeFrames.push_back(flowTimeFrame);
				}

				result.emplace(day, matrix);
			}

			file.close();

			return result;
		}
	};
}