#pragma once

#include "../../CityBikes.Data/fill-level/fill-level-prediction.hpp"

namespace CityBikes::DataProcessing::Validation
{
	template <size_t Nodes>
	class AvailabilityValidation
	{
	public:
		static double getTimeOfUnavailability(const Data::FillLevel::FillLevelPrediction<Nodes>& model)
		{
			size_t timeOfUnavailability = 0;

			for (const auto & timeFrame : model)
				for (const auto& node : timeFrame)
					if (node < 1)
						timeOfUnavailability++;

			return (double)timeOfUnavailability / Nodes;
		}
	};
}