#pragma once

#include "relocation-operation.hpp"

#include <vector>

namespace CityBikes::Data::Relocation
{
	struct RelocationSchedule : public std::vector<RelocationOperation>
	{ };
}