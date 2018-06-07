#pragma once

#include "../../CityBikes.Data/fill-level/fill-level-prediction.hpp"

namespace CityBikes::DataProcessing::FillLevel
{
	template<size_t Nodes>
	class FillLevelProcessing
	{
	public:
		static Data::FillLevel::FillLevelPrediction<Nodes> average(
			const std::vector<Data::FillLevel::FillLevelPrediction<Nodes>>& predictions
		)
		{
			Data::FillLevel::FillLevelPrediction<Nodes> finalPrediction;

			for (size_t frame = 0; frame < predictions[0].size(); frame++)
			{
				Data::FillLevel::FillLevelPredictionFrame<Nodes> predictionFrame;
				for (auto& value : predictionFrame)
					value = 0;

				for (const auto &prediction : predictions)
					for (size_t node = 0; node < Nodes; node++)
						predictionFrame[node] += prediction[frame][node] / predictions.size();

				finalPrediction.push_back(predictionFrame);
			}

			return finalPrediction;
		}
	};
}