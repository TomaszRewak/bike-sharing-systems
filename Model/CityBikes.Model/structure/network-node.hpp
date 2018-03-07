#pragma once

#include <algorithm>

namespace CityBikes::Model::Structure
{
	struct NetworkNode
	{
		double value;

		NetworkNode() :
			value(0)
		{ }

		void add(double delta)
		{
			value += delta;
		}

		double get(double delta, bool force = false)
		{
			if (!force)
				delta = std::min(delta, value);

			value -= delta;

			return delta;
		}
	};
}