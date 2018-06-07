#pragma once

#include "../../CityBikes.Data/fill-level/fill-level-prediction.hpp"

namespace CityBikes::DataProcessing::FillLevel
{
	template<size_t Nodes>
	class Error
	{
	public:
		static double computeOffset(
			Data::FillLevel::FillLevelPrediction<Nodes>& modelA,
			Data::FillLevel::FillLevelPrediction<Nodes>& modelB)
		{
			double diff = 0;

			for (size_t node = 0; node < Nodes; node++)
				for (size_t timeFrame = 0; timeFrame < modelA.size(); timeFrame++)
					diff += std::pow(modelA[timeFrame][node] - modelB[timeFrame][node], 2);

			diff /= modelA.size() * Nodes;

			return diff;
		}

		static double computeWindowedOffset(
			Data::FillLevel::FillLevelPrediction<Nodes>& modelA,
			Data::FillLevel::FillLevelPrediction<Nodes>& modelB,
			size_t window)
		{
			throw "Not implemented exception";
		}
	};
}