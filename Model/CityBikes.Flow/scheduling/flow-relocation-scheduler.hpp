#pragma once

#include "../flow-relocation-model.hpp"
#include "../relocation/relocation-task.hpp"
#include "../../CityBikes.Model/flow-distribution-model.hpp"
#include "../filling/network-filling-matrix.hpp"

namespace CityBikes::Flow::Scheduling
{
	class FlowRelocationScheduler
	{
	public:
		/// <summary> Make best decisions for a given (current) time frame </summary>
		void schedule(
			FlowRelocationModel &relocationModel,
			Model::FlowDistributionModel &distributionModel)
		{
			// Prepare simulation data

			Filling::NetworkFillingMatrix fillingMatrix(distributionModel);

			// TODO: Check if any relocation unit is awaiting

			// Apply ongoing relocations

			for (auto& relocationUnit : relocationModel.relocationUnits)
			{
				if (relocationUnit.positionReachTime <= relocationModel.timeFrame)
					continue;

				applyRelocationUnitRoute(fillingMatrix, relocationUnit);
			}

			// Schedule new relocations
		}
	private:
		/// <summary> Changes the NetworkFillingMatrix based on already established relocation unit route </summary>
		void applyRelocationUnitRoute(Filling::NetworkFillingMatrix& fillingMatrix, Relocation::RelocationUnit& relocationUnit)
		{

		}
	};
}