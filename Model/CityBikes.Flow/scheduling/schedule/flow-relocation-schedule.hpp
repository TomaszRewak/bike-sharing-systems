#pragma once

#include <vector>

#include "../../decision-making/decision/scored-relocation-operation.hpp"

namespace CityBikes::Flow::Scheduling::Schedule
{
	struct FlowRelocationSchedule
	{
		/// <summary> The higher the score - the better </summary>
		int score = 0;

		std::vector<Relocation::RelocationOperation> operations;

		void add(DecisionMaking::Decision::ScoredRelocationOperation& scoredOperation)
		{
			operations.push_back(scoredOperation.operation);

			score += scoredOperation.score;
		}
	};
}