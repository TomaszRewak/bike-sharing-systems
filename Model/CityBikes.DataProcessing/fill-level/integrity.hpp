#include "../../CityBikes.Data/fill-level/fill-level-prediction-frame.hpp"

namespace CityBikes::DataProcessing::FillLevel
{
	template<size_t Nodes>
	class Integrity
	{
	public:
		static double computeTotalNumber(Data::FillLevel::FillLevelPredictionFrame<Nodes>& frame)
		{
			double totalValue = 0;

			for (auto& node : frame)
				totalValue += node;

			return totalValue;
		}
	};
}