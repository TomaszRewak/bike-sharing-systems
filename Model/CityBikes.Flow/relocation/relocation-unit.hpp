#pragma once

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		float load;

		size_t targetPosition;
		size_t targetPositionReachTime;
	};
}