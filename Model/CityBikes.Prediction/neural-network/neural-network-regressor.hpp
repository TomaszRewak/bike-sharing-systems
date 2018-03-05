#pragma once

#include <vector>

namespace CityBikes::Prediction::NeuralNetwork
{
	class NeuralNetworkRegressor
	{
	private:
		std::vector<std::vector<std::vector<double>>> coefficients;
		std::vector<std::vector<double>> biases;

	public:
		NeuralNetworkRegressor(
			std::vector<std::vector<std::vector<double>>> coefficients,
			std::vector<std::vector<double>> biases
		):
			coefficients(coefficients),
			biases(biases)
		{ }

		double predict(std::vector<double>& values)
		{
			std::vector<double> oldValues = values;
			std::vector<double> newValues;

			for (size_t layer = 0; ; layer++)
			{
				auto& layer_coefficients = coefficients[layer];
				auto& layer_biases = biases[layer];

				newValues.clear();
				newValues.resize(layer_biases.size(), 0);

				for (size_t oldNode = 0; oldNode < oldValues.size(); oldNode++)
					for (size_t newNode = 0; newNode < newValues.size(); newNode++)
						newValues[newNode] += oldValues[oldNode] * layer_coefficients[oldNode][newNode];

				for (size_t newNode = 0; newNode < newValues.size(); newNode++)
					newValues[newNode] += layer_biases[newNode];

				if (layer == biases.size() - 1)
					break;

				for (auto& value : newValues)
					value = std::max(value, 0.); // relu

				std::swap(oldValues, newValues);
			}

			return newValues[0];
		}
	};
}