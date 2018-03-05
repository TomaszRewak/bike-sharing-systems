#pragma once

#include <experimental/filesystem>
#include <fstream>

#include "neural-network-regressor.hpp"

namespace CityBikes::Prediction::NeuralNetwork
{
	class NeuralNetworkReader
	{
	public:
		static NeuralNetworkRegressor read(std::experimental::filesystem::path path)
		{
			std::ifstream file(path);

			size_t numberOfLayers;
			file >> numberOfLayers;

			std::vector<size_t> layerSizes;
			for (size_t layer = 0; layer < numberOfLayers; layer++)
			{
				size_t layerSize;
				file >> layerSize;
				layerSizes.push_back(layerSize);
			}

			std::vector<std::vector<std::vector<double>>> coefficients;

			for (size_t layer = 0; layer < numberOfLayers - 1; layer++)
			{
				std::vector<std::vector<double>> layerCoefficients;
				
				for (size_t node = 0; node < layerSizes[layer]; node++)
				{
					std::vector<double> nodeCoefficients;

					for (size_t nextNode = 0; nextNode < layerSizes[layer + 1]; nextNode++)
					{
						double coefficient;
						file >> coefficient;
						nodeCoefficients.push_back(coefficient);
					}

					layerCoefficients.push_back(nodeCoefficients);
				}

				coefficients.push_back(layerCoefficients);
			}

			std::vector<std::vector<double>> biases;

			for (size_t layer = 1; layer < numberOfLayers; layer++)
			{
				std::vector<double> layerBiases;

				for (size_t node = 0; node < layerSizes[layer]; node++)
				{
					double bias;
					file >> bias;
					layerBiases.push_back(bias);
				}

				biases.push_back(layerBiases);
			}

			file.close();

			return NeuralNetworkRegressor(coefficients, biases);
		}
	};
}