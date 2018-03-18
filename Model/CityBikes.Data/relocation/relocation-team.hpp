#pragma once

#include "relocation-unit.hpp"

#include <vector>

namespace CityBikes::Data::Relocation
{
	struct RelocationTeam : public std::vector<RelocationUnit>
	{ };
}