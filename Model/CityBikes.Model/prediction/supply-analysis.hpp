#pragma once

#include "../../CityBikes.Data/supply/supply-prediction.hpp"
#include "../../CityBikes.Data/flow/flow-action.hpp"

#include <algorithm>

namespace CityBikes::Model::Prediction
{
	template<size_t Nodes>
	class SupplyAnalysis
	{
	private:
		static void registerAction(Data::Supply::SupplyPredictionMatrix<Nodes>& supplyMatrix, const Data::Flow::FlowAction& action)
		{
			supplyMatrix[action.flowInstance.source.node][action.flowInstance.destination.node][action.flowInstance.destination.timeFrame - action.flowInstance.source.timeFrame] += action.actionProbability;
		}

		static void normalizeSupplyMatrix(Data::Supply::SupplyPredictionMatrix<Nodes>& supplyMatrix)
		{
			for (auto& source : supplyMatrix)
			{
				double sum = 0;

				for (auto& destination : source)
					for (auto& supply : destination)
						sum += supply.second;

				for (auto& destination : source)
					for (auto& supply : destination)
						supply.second /= sum;
			}
		}

	public:
		static Data::Supply::SupplyPrediction<Nodes> computeFullSupply(std::vector<Data::Flow::FlowAction> actions, size_t timeFrames)
		{
			Data::Supply::SupplyPrediction<Nodes> supplyPrediction(timeFrames);

			for (const Data::Flow::FlowAction& action : actions)
				registerAction(supplyPrediction[action.flowInstance.source.timeFrame], action);

			for (Data::Supply::SupplyPredictionMatrix<Nodes>& supplyMatrix : supplyPrediction)
				normalizeSupplyMatrix(supplyMatrix);

			return supplyPrediction;
		}

		static Data::Supply::SupplyPrediction<Nodes> computeAvgSupply(std::vector<Data::Flow::FlowAction> actions, size_t timeFrames)
		{
			Data::Supply::SupplyPrediction<Nodes> supplyPrediction(timeFrames);
			CityBikes::Data::Supply::SupplyPredictionMatrix<Nodes> supplyMatrix;

			for (const Data::Flow::FlowAction& action : actions)
				registerAction(supplyMatrix, action);

			normalizeSupplyMatrix(supplyMatrix);

			for (CityBikes::Data::Supply::SupplyPredictionMatrix<Nodes>& matrix : supplyPrediction)
				matrix = supplyMatrix;

			return supplyPrediction;
		}
	};
}