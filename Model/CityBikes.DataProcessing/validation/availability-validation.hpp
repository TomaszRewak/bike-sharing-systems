#pragma once

#include "../../CityBikes.Model/flow-distribution-model.hpp"

namespace CityBikes::DataProcessing::Validation
{
	class AvailabilityValidation
	{
	public:
		template <size_t Nodes>
		static double getTimeOfUnavailability(const Model::FlowDistributionModel<Nodes>& model)
		{
			size_t timeOfUnavailability = 0;

			for (const auto & timeFrame : model.timeFrames)
				for (const auto& node : timeFrame.nodes)
					if (node.value < 1)
						timeOfUnavailability++;

			return (double)timeOfUnavailability / Nodes;
		}
	};
}