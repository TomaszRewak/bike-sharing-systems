#pragma once

#include "../../CityBikes.Data/demand/cumulative-demand-prediction.hpp"
#include "../../CityBikes.Data/time/day.hpp"

namespace CityBikes::DataProcessing::Demand
{
	template<size_t Nodes>
	class DemandPredictionProcessing
	{
	public:
		static Data::Demand::CumulativeDemandPrediction<Nodes> discretize(Data::Demand::CumulativeDemandPrediction<Nodes> demand)
		{
			for (auto& frame : demand)
				for (auto& node : frame)
					node = (int)node;

			return demand;
		}
	};
}