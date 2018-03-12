#pragma once

namespace CityBikes::Flow::Configuration
{
	struct RelocationSchedulingConfiguration
	{
		size_t predictionWindow;

		RelocationSchedulingConfiguration(size_t predictionWindow):
			predictionWindow(predictionWindow)
		{ }
	};
}