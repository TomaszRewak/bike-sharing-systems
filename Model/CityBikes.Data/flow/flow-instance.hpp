#pragma once

#include "flow-target.hpp"

namespace CityBikes::Data::Flow
{
	struct FlowInstance
	{
		FlowTarget source;
		FlowTarget destination;
	};
}