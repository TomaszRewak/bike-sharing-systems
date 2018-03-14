#pragma once

#include "../../relocation/relocation-operation.hpp"

namespace CityBikes::Flow::DecisionMaking::Decision
{
	struct FillDecision
	{
		Relocation::RelocationOperation operation;
		size_t score;

		FillDecision(size_t node):
			operation(node, 0),
			score(0)
		{ }

		FillDecision(size_t node, int nodeFillChange, size_t score) :
			operation(node, nodeFillChange),
			score(score)
		{ }
	};
}