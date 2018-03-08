#pragma once

#include "../schedule/flow-relocation-schedule.hpp"
#include "../../filling/network-filling-matrix.hpp"
#include "../../filling/network-filling-matrix-alteration.hpp"
#include "../../decision-making/fill-greedy-algorithm.hpp"
#include "../../decision-making/direction-greedy-algorithm.hpp"
#include "../../relocation/relocation-unit.hpp"
#include "../../../CityBikes.Data/flow/flow-matrix.hpp"

namespace CityBikes::Flow::Scheduling::Space
{
	/// <summary> Space of possible solutions - routs of relocation units </summary>
	class FlowRelocationSchedulingSpace
	{
	private:
		Filling::NetworkFillingMatrix& networkFillingMatrix;
		DecisionMaking::FillGreedyAlgorithm& fillGreedyAlgorithm;
		DecisionMaking::DirectionGreedyAlgorithm& directionGreedyAlgorithm;

		size_t routeLength;

	public:
		FlowRelocationSchedulingSpace(
			Filling::NetworkFillingMatrix &networkFillingMatrix,
			DecisionMaking::FillGreedyAlgorithm& fillGreedyAlgorithm,
			DecisionMaking::DirectionGreedyAlgorithm& directionGreedyAlgorithm,
			size_t routeLength
		) :
			networkFillingMatrix(networkFillingMatrix),
			fillGreedyAlgorithm(fillGreedyAlgorithm),
			directionGreedyAlgorithm(directionGreedyAlgorithm),
			routeLength(routeLength)
		{ }

		/// <summary> Stochastically produces a new instance of a relocation unit route based on greedy algorithm </summary>
		Schedule::FlowRelocationSchedule getNext(Relocation::RelocationUnit relocationUnit)
		{
			// prepare data

			Schedule::FlowRelocationSchedule schedule;

			Filling::NetworkFillingMatrixAlteration alteration(networkFillingMatrix);

			// make first decision (based on current direction or current position of the car)

			auto bestFirstDecision = fillGreedyAlgorithm.makeDecision(
				alteration,
				relocationUnit
			);

			schedule.add(bestFirstDecision);

			// Apply changes

			applyOperation(bestFirstDecision.operation, relocationUnit, alteration);

			// make other decisions

			for (size_t i = 0; i < routeLength; i++)
			{
				// Make best decision about direction

				auto directionDecision = directionGreedyAlgorithm.getDecision(
					alteration,
					relocationUnit
				);

				schedule.add(directionDecision.first);
				schedule.add(directionDecision.second);

				// Apply changes

				applyOperation(directionDecision.first.operation, relocationUnit, alteration);
				applyOperation(directionDecision.second.operation, relocationUnit, alteration);
			}

			// return result

			return schedule;
		}

	private:
		void applyOperation(Relocation::RelocationOperation& operation, Relocation::RelocationUnit& relocationUnit, Filling::NetworkFillingMatrixAlteration& alteration)
		{
			relocationUnit.schedule(operation);
			alteration[operation.destination] += operation.destinationFillChange;
		}
	};
}