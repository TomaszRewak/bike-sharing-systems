#pragma once

namespace CityBikes::Flow::Configuration
{
	/// <summary> Limits for minimal and maximal threshold. Values outside these bounds should be avoided. </summary>
	struct ThresholdConfiguration
	{
		int minValue;
		int maxValue;

		ThresholdConfiguration(int minValue, int maxValue) :
			minValue(minValue),
			maxValue(maxValue)
		{ }
	};
}