#pragma once

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Decision made by DirectionGreedyAlgorithm </summary>
	struct DirectionDecision
	{
		/// <summary> Target node </summary>
		size_t direction;
		/// <summary> Already computed flow duration </summary>
		size_t flowDuration;
		/// <summary> Score of the decision based on best possible greedy decision made in next step </summary>
		size_t score;

		DirectionDecision(size_t direction, size_t flowDuration, size_t score):
			direction(direction),
			flowDuration(flowDuration),
			score(score)
		{ }
	};
}