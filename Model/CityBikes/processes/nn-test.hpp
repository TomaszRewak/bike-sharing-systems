#pragma once

#include <iostream>

#include "../../CityBikes.Prediction/neural-network/neural-network-reader.hpp"

namespace CityBikes::Processes
{
	void testNeuralNetwork()
	{
		auto nn = Prediction::NeuralNetwork::NeuralNetworkReader::read("../../Resources/learning/test.nn");

		std::vector<double> examplesValues
		{
			0.2, 13., 3.3, 0.2, 6., 0.12, 4., 0.1, 0.3, 2.1, 0.4, 3.1, 0.5, 6.2, 2.7, 0.2
		};

		double preditedValue = nn.predict(examplesValues);

		std::cout << preditedValue << std::endl;

		std::getchar();
	}
}