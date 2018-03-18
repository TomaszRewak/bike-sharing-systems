#pragma once

namespace CityBikes::Data::Relocation
{
	struct RelocationOperation
	{
		size_t destination;
		int nodeFillChange;

		RelocationOperation(size_t destination, int nodeFillChange) :
			destination(destination),
			nodeFillChange(nodeFillChange)
		{ }
	};
}