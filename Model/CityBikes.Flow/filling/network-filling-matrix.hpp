#pragma once

#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "structure/network-filling.hpp"

namespace CityBikes::Flow::Filling
{
	class NetworkFillingMatrix
	{
	public:
		const size_t nodes;

	private:
		Model::FlowDistributionModel baseModel;

		std::vector<Structure::NodeFillingDefinition> nodeFillingDefinitions;
		std::vector<Structure::NetworkFilling> timeFrames;

	public:
		NetworkFillingMatrix(Model::FlowDistributionModel& model) :
			baseModel(model),
			nodes(model.nodes)
		{
			size_t timeFramesNumber = baseModel.timeFrames.size();
			size_t nodesNumber = baseModel.nodes;

			// compute node limits

			nodeFillingDefinitions = std::vector<Structure::NodeFillingDefinition>(nodesNumber);

			for (size_t timeFrame = 0; timeFrame < timeFramesNumber; timeFrame++)
			{
				for (size_t node = 0; node < nodesNumber; node++)
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

			Structure::NetworkFilling networkFilling(nodeFillingDefinitions);

			for (int timeFrame = timeFramesNumber - 1; timeFrame >= 0; timeFrame--)
			{
				for (size_t node = 0; node < nodesNumber; node++)
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

		Structure::NodeFillingDefinition getNodeFillingDefinition(size_t node)
		{
			return nodeFillingDefinitions[node];
		}
	};
}