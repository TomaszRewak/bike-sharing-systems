#pragma once

#include "../schedule/flow-relocation-schedule.hpp"
#include "../../filling/network-filling-matrix.hpp"
#include "../../filling/network-filling-matrix-alteration.hpp"
#include "../../decision-making/fill-greedy-algorithm.hpp"
#include "../../decision-making/direction-greedy-algorithm.hpp"
#include "../../relocation/relocation-unit.hpp"

namespace CityBikes::Flow::Scheduling::Space
{
	/// <summary> Space of possible solutions - routs of relocation units </summary>
	template<size_t Nodes>
	class FlowRelocationSchedulingSpace
	{
	private:
		const Filling::NetworkFillingMatrix<Nodes>& networkFillingMatrix;
		DecisionMaking::FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm;
		DecisionMaking::DirectionGreedyAlgorithm<Nodes>& directionGreedyAlgorithm;

		size_t routeLength;
		size_t timeLength;

	public:
		FlowRelocationSchedulingSpace(
			const Filling::NetworkFillingMatrix<Nodes> &networkFillingMatrix,
			DecisionMaking::FillGreedyAlgorithm<Nodes>& fillGreedyAlgorithm,
			DecisionMaking::DirectionGreedyAlgorithm<Nodes>& directionGreedyAlgorithm,
			size_t routeLength,
			size_t timeLength
		) :
			networkFillingMatrix(networkFillingMatrix),
			fillGreedyAlgorithm(fillGreedyAlgorithm),
			directionGreedyAlgorithm(directionGreedyAlgorithm),
			routeLength(routeLength),
			timeLength(timeLength)
		{ }

		/// <summary> Stochastically produces a new instance of a relocation unit route based on greedy algorithm </summary>
		Schedule::FlowRelocationSchedule getNext(Relocation::RelocationUnit relocationUnit)
		{
			// prepare data

			Schedule::FlowRelocationSchedule schedule;

			Filling::NetworkFillingMatrixAlteration<Nodes> alteration(networkFillingMatrix);

			// make first decision (based on current direction or current position of the car)

			auto bestFirstDecision = fillGreedyAlgorithm.makeDecision(
				alteration,
				relocationUnit
			);

			schedule.add(bestFirstDecision);

			// Apply changes

			applyOperation(bestFirstDecision.operation, relocationUnit, alteration);

			// make other decisions

			for (size_t i = 0; i < routeLength && relocationUnit.currentOperation.remainingTime < timeLength; i++)
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
		void applyOperation(Relocation::RelocationOperation& operation, Relocation::RelocationUnit& relocationUnit, Filling::NetworkFillingMatrixAlteration<Nodes>& alteration)
		{
			relocationUnit.schedule(operation);
			alteration[operation.destination] += operation.destinationFillChange;
		}
	};
}