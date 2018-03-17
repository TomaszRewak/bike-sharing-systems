#pragma once

#include <map>

namespace CityBikes::Data::Supply
{
	struct SupplyPredictionDistribution : std::map<size_t, double>
	{ };
}