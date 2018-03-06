#pragma once

namespace CityBikes::Flow::Relocation
{
	/// <summary> Relocation limit as defined for a station (not for a unit) </summary>
	struct RelocationLimit
	{
		int minChange;
		int maxChange;

		RelocationLimit(int minChange, int maxChange) :
			minChange(minChange),
			maxChange(maxChange)
		{ }
	};
}