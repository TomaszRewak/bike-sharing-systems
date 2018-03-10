#pragma once

#include <filesystem>
#include <fstream>
#include <iomanip>

#include "../../../CityBikes.Model/flow-distribution-model.hpp"

namespace CityBikes::Data::Predictions::Utils
{
	class PredictionWriter
	{
	private:
		template<size_t Nodes>
		static void writeHeader(std::ostream& file, Model::FlowDistributionModel<Nodes>& model)
		{
			file << Nodes << " " << model.timeFrames.size() << std::endl;
		}

		template<size_t Nodes>
		static void writePrediction(std::ostream& file, Model::FlowDistributionModel<Nodes>& model)
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
		template<size_t Nodes>
		static void writeData(Model::FlowDistributionModel<Nodes>& model, std::experimental::filesystem::path path)
		{
			std::ofstream file(path);

			writeHeader(file, model);
			writePrediction(file, model);

			file.close();
		}
	};
}