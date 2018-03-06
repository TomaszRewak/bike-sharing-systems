#pragma once

namespace CityBikes::Flow::DecisionMaking
{
	/// <summary> Decision concluded by the greedy algorithm for a single step </summary>
	struct FillChangeDecision
	{
		/// <summary> Change of the fillness of the station (not the relocation unit!)  </summary>
		int fillChange;
		/// <summary> The higher the score the better </summary>
		size_t score;

		FillChangeDecision(int fillChange, size_t score):
			fillChange(fillChange),
			score(score)
		{ }
	};
}