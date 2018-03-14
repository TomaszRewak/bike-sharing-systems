#pragma once

namespace CityBikes::Flow::Relocation
{
	struct RelocationOperation
	{
		size_t destination;
		int change;

		RelocationOperation(size_t destination, int change) :
			destination(destination),
			change(change)
		{ }
	};
}