#pragma once

#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "structure/network-filling.hpp"

namespace CityBikes::Flow::Filling
{
	template<size_t Nodes>
	class NetworkFillingMatrix
	{
	private:
		Model::FlowDistributionModel<Nodes> baseModel;

		std::array<Structure::NodeFillingDefinition, Nodes> nodeFillingDefinitions;

		std::vector<Structure::NetworkFilling<Nodes>> timeFrames;

	public:
		NetworkFillingMatrix(Model::FlowDistributionModel<Nodes>& model) :
			baseModel(model)
		{
			size_t timeFramesNumber = baseModel.timeFrames.size();

			// compute node limits

			for (size_t timeFrame = 0; timeFrame < timeFramesNumber; timeFrame++)
			{
				for (size_t node = 0; node < Nodes; node++)
				{
					int number = getNumber(timeFrame, node);

					nodeFillingDefinitions[node].minNumber = std::min(
						nodeFillingDefinitions[node].minNumber,
						number
					);

					nodeFillingDefinitions[node].maxNumber = std::max(
						nodeFillingDefinitions[node].maxNumber,
						number
					);
				}
			}

			// compute node fillings

			Structure::NetworkFilling<Nodes> networkFilling(nodeFillingDefinitions);

			for (int timeFrame = timeFramesNumber - 1; timeFrame >= 0; timeFrame--)
			{
				for (size_t node = 0; node < Nodes; node++)
				{
					int number = getNumber(timeFrame, node);

					networkFilling.setFilling(
						node,
						number,
						networkFilling.getFilling(node, number) + 1
					);
				}

				timeFrames.push_back(networkFilling);
			}

			std::reverse(timeFrames.begin(), timeFrames.end());
		}

		int getNumber(size_t timeFrame, size_t node)
		{
			return std::round(baseModel.timeFrames[timeFrame].nodes[node].value);
		}

		size_t getAboveThreshold(size_t timeFrame, size_t node, int threshold)
		{
			if (timeFrame > timeFrames.size())
				return 0;

			size_t count = 0;

			for (int number = threshold + 1; number <= nodeFillingDefinitions[node].maxNumber; number++)
				count += timeFrames[timeFrame].getFilling(node, number) * (number - threshold);

			return count;
		}

		size_t getBelowThreshold(size_t timeFrame, size_t node, int threshold)
		{
			if (timeFrame > timeFrames.size())
				return 0;

			size_t count = 0;

			for (int number = threshold - 1; number >= nodeFillingDefinitions[node].minNumber; number--)
				count += timeFrames[timeFrame].getFilling(node, number) * (threshold - number);

			return count;
		}
	};
}