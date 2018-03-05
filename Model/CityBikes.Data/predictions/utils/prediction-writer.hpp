#pragma once

#include <filesystem>
#include <fstream>
#include <iomanip>

#include "../../CityBikes.Model/flow-distribution-model.hpp"

namespace CityBikes::Data::Predictions::Utils
{
	class PredictionWriter
	{
	private:
		static void writeHeader(std::ostream& file, Model::FlowDistributionModel& model)
		{
			file << model.timeFrames[0].nodes.size() << " " << model.timeFrames.size() << std::endl;
		}

		static void writePrediction(std::ostream& file, Model::FlowDistributionModel& model)
		{
			for (auto& timeFrame : model.timeFrames)
			{
				for (auto& node : timeFrame.nodes)
				{
					file << node.value << "\t";
				}

				file << std::endl;
			}
		}

	public:
		static void writeData(Model::FlowDistributionModel& model, std::experimental::filesystem::path path)
		{
			std::ofstream file(path);

			writeHeader(file, model);
			writePrediction(file, model);

			file.close();
		}
	};
}