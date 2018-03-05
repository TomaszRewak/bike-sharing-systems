#pragma once

#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "structure/network-filling.hpp"
#include "network-filling-matrix-alteration.hpp"

namespace CityBikes::Flow::Validation
{
	class NetworkFillingMatrix
	{
	private:
		std::vector<Structure::NodeFillingDefinition> nodeFillingDefinitions;
		std::vector<Structure::NetworkFilling> timeFrames;

	public:
		NetworkFillingMatrix(Model::FlowDistributionModel& model)
		{
			size_t timeFramesNumber = model.timeFrames.size();
			size_t nodesNumber = model.timeFrames[0].nodes.size();

			nodeFillingDefinitions = std::vector<Structure::NodeFillingDefinition>(nodesNumber);

			for (size_t timeFrame = 0; timeFrame < timeFramesNumber; timeFrame++)
			{
				for (size_t node = 0; node < nodesNumber; node++)
				{
					int number = std::round(model.timeFrames[timeFrame].nodes[node].value);

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

			Structure::NetworkFilling networkFilling(nodeFillingDefinitions);

			for (int timeFrame = timeFramesNumber - 1; timeFrame >= 0; timeFrame--)
			{
				for (size_t node = 0; node < nodesNumber; node++)
				{
					int number = std::round(model.timeFrames[timeFrame].nodes[node].value);

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

		size_t getAboveThreshold(NetworkFillingMatrixAlteration& alteration, size_t timeFrame, size_t node, int threshold)
		{
			size_t count = 0;

			threshold -= alteration.getAlteration(node);

			for (int number = threshold + 1; number <= nodeFillingDefinitions[node].maxNumber; number++)
				count += timeFrames[timeFrame].getFilling(node, number) * (number - threshold);

			return count;
		}

		size_t getBelowThreshold(NetworkFillingMatrixAlteration& alteration, size_t timeFrame, size_t node, int threshold)
		{
			size_t count = 0;

			threshold -= alteration.getAlteration(node);

			for (int number = threshold - 1; number >= nodeFillingDefinitions[node].minNumber; number--)
				count += timeFrames[timeFrame].getFilling(node, number) * (threshold - number);

			return count;
		}
	};
}