#pragma once

#include <array>

namespace CityBikes::Data::FlowTime
{
	template<size_t Nodes>
	struct FlowTimePredictionMatrix : public std::array<std::array<float, Nodes>, Nodes>
	{ };
}