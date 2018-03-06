#pragma once

#include "flow-relocation-schedule.hpp"
#include "../filling/network-filling-matrix.hpp"
#include "../filling/network-filling-matrix-alteration.hpp"
#include "../decision-making/fill-greedy-algorithm.hpp"
#include "../decision-making/direction-greedy-algorithm.hpp"
#include "../relocation/relocation-unit.hpp"
#include "../../CityBikes.Data/flow/flow-matrix.hpp"

namespace CityBikes::Flow::Scheduling
{
	/// <summary> Space of possible solutions - routs of relocation units </summary>
	class FlowRelocationSchedulingSpace
	{
	private:
		Filling::NetworkFillingMatrix networkFillingMatrix;
		DecisionMaking::FillGreedyAlgorithm fillGreedyAlgorithm;
		DecisionMaking::DirectionGreedyAlgorithm directionGreedyAlgorithm;

	public:
		FlowRelocationSchedulingSpace(
			Model::FlowDistributionModel &distributionModel,
			Data::Flow::FlowMatrix &flowMatrix,
			Configuration::ThresholdConfiguration thresholdConfiguration
		) :
			networkFillingMatrix(distributionModel),
			fillGreedyAlgorithm(thresholdConfiguration),
			directionGreedyAlgorithm(flowMatrix, fillGreedyAlgorithm)
		{ }

		/// <summary> Stochastically produces a new instance of a relocation unit route based on greedy algorithm </summary>
		FlowRelocationSchedule getNext(Relocation::RelocationUnitState relocationUnit, size_t length)
		{
			size_t timeFrame = 0;

			FlowRelocationSchedule result;

			Filling::NetworkFillingMatrixAlteration alteration(networkFillingMatrix);

			for (size_t i = 0; i < length; i++)
			{
				// Make best possible desiciton about station fillness

				auto bestDecision = fillGreedyAlgorithm.makeDecision(
					alteration,
					timeFrame,
					relocationUnit.currentPosition,
					relocationUnit.getRelocationLimit()
				);

				// Apply filness change decision

				alteration.alter(
					relocationUnit.currentPosition,
					bestDecision.fillChange
				);

				relocationUnit.currentLoad -= bestDecision.fillChange;

				// Make best decision about direction

				auto directionDecision = directionGreedyAlgorithm.getDecision(
					alteration,
					relocationUnit,
					networkFillingMatrix.startFrame(),
					timeFrame
				);

				// Apply direction decision

				timeFrame += directionDecision.flowDuration;
				relocationUnit.currentPosition = directionDecision.direction;

				result.score += directionDecision.score;
				result.route.push_back(directionDecision.flowDuration);
			}

			return result;
		}
	};
}