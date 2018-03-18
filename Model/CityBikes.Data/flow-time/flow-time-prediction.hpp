#pragma once

#include <vector>

#include "flow-time-prediction-matrix.hpp"

namespace CityBikes::Data::FlowTime
{
	/// <summary> Describes flow time between nodes for next time frames </summary>
	template<size_t Nodes>
	struct FlowTimePrediction : public std::vector<FlowTimePredictionMatrix<Nodes>>
	{
		FlowTimePrediction(size_t timeFrames) :
			std::vector<FlowTimePredictionMatrix<Nodes>>(timeFrames)
		{ }

		double flowDuration(size_t source, size_t destination, size_t timeFrame) const
		{
			timeFrame = std::min(timeFrame, this->size() - 1);

			return (*this)[timeFrame][source][destination];
		}
	};
}