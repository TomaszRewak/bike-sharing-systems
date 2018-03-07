#pragma once

#include "fill-change-decision.hpp"

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Decision made by DirectionGreedyAlgorithm </summary>
	struct DirectionDecision : public FillChangeDecision
	{
		/// <summary> Target node </summary>
		size_t direction;
		/// <summary> Already computed flow duration </summary>
		size_t flowDuration;

		DirectionDecision(size_t direction, size_t flowDuration, int fillChange, size_t score):
			FillChangeDecision(fillChange, score),
			direction(direction),
			flowDuration(flowDuration)
		{ }
	};
}