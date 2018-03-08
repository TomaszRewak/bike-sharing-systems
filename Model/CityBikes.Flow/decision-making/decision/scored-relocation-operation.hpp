#pragma once

#include "../../relocation/relocation-operation.hpp"

namespace CityBikes::Flow::DecisionMaking::Decision
{
	struct ScoredRelocationOperation
	{
		/// <summary> Scored operation </summary>
		Relocation::RelocationOperation operation;

		/// <summary> The higher the score the better </summary>
		size_t score;

		ScoredRelocationOperation(Relocation::RelocationOperation operation, size_t score) :
			operation(operation),
			score(score)
		{ }
	};
}