#pragma once

#include <vector>

#include "../../relocation/relocation-operation.hpp"

namespace CityBikes::Flow::DecisionMaking::Decision
{
	struct PathDecision
	{
		std::vector<Relocation::RelocationOperation> operations;
		size_t score;

		PathDecision() :
			score(0)
		{ }
	};
}