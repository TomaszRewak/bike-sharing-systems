#pragma once

#include "flow-relocation-model.hpp"
#include "relocation/relocation-task.hpp"
#include "../CityBikes.Model/flow-distribution-model.hpp"
#include "validation/network-filling-matrix.hpp"

#include <optional>

namespace CityBikes::Flow
{
	class FlowRelocationScheduling
	{
	public:
		static std::vector<std::optional<Relocation::RelocationTask>> schedule(
			FlowRelocationModel &relocationModel,
			Model::FlowDistributionModel &distributionModel)
		{
			Validation::NetworkFillingMatrix fillingMatrix(distributionModel);

			for (size_t i_unit = 0; i_unit < relocationModel.relocationUnits.size(); i_unit++)
			{
				auto& unit = relocationModel.relocationUnits[i_unit];


			}
		}
	};
}