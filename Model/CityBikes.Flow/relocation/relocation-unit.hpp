#pragma once

namespace CityBikes::Flow::Relocation
{
	struct RelocationUnit
	{
		float currentLoad;
		std::vector<size_t> plannedPath;
	};
}