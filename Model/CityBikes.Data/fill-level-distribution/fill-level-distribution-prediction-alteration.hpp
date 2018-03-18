#pragma once

#include "fill-level-distribution-prediction.hpp"

namespace CityBikes::Data::FillLevelDistribution
{
	/// <summary> Changes the fillness of nodes for all time frames simultaneously without a need for a recalculation </summary>
	template<size_t Nodes>
	class FillLevelDistributionPredictionAlteration : public std::array<int, Nodes>
	{
	private:
		const FillLevelDistributionPrediction<Nodes> & prediction;

	public:
		FillLevelDistributionPredictionAlteration(const FillLevelDistributionPrediction<Nodes> & prediction) :
			std::array<int, Nodes>{},
			prediction(prediction)
		{ }

		int getValue(size_t timeFrame, size_t node) const
		{
			return prediction[timeFrame][node].baseValue + (*this)[node];
		}

		size_t getAboveThreshold(size_t timeFrame, size_t node, int threshold) const
		{
			threshold -= (*this)[node];

			return prediction[timeFrame][node].getAboveThreshold(threshold);
		}

		size_t getBelowThreshold(size_t timeFrame, size_t node, int threshold) const
		{
			threshold -= (*this)[node];

			return prediction[timeFrame][node].getBelowThreshold(threshold);
		}

		size_t timeFrames() const
		{
			return prediction.size();
		}
	};
}