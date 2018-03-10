#pragma once

#include <array>

namespace CityBikes::Data::FlowTime::Structure
{
	template<size_t Nodes>
	struct FlowTimeMatrixFrame : public std::array<std::array<size_t, Nodes>, Nodes>
	{ };
}