#pragma once

#include "flow-target.hpp"

namespace CityBikes::Model::Data
{
	struct FlowInstance
	{
		FlowTarger source;
		FlowTarger destination;
	};
}