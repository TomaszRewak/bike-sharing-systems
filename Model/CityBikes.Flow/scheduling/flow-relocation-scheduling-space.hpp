#pragma once

#include "schedule/flow-relocation-schedule.hpp"
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
		size_t routeLength;

	public:
		FlowRelocationSchedulingSpace(
			Filling::NetworkFillingMatrix &networkFillingMatrix,
			Data::Flow::FlowMatrix &flowMatrix,
			Configuration::ThresholdConfiguration thresholdConfiguration,
			size_t routeLength
		) :
			networkFillingMatrix(networkFillingMatrix),
			fillGreedyAlgorithm(thresholdConfiguration),
			directionGreedyAlgorithm(flowMatrix, fillGreedyAlgorithm),
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

			schedule.steps.push_back(Schedule::FlowRelocationScheduleStep(
				relocationUnit.position,
				bestFirstDecision.fillChange,
				relocationUnit.timeUntilAvailable
			));

			// Apply changes

			relocationUnit.state.currentLoad -= bestFirstDecision.fillChange;
			alteration[relocationUnit.position] += bestFirstDecision.fillChange;

			// make other decisions

			for (size_t i = 0; i < routeLength; i++)
			{
				// Make best decision about direction

				auto directionDecision = directionGreedyAlgorithm.getDecision(
					alteration,
					relocationUnit
				);

				// Apply changes

				relocationUnit.state.currentLoad -= directionDecision.fillChange;
				alteration[relocationUnit.position] += directionDecision.fillChange;

				relocationUnit.position = directionDecision.direction;
				relocationUnit.timeUntilAvailable += directionDecision.flowDuration;

				// Apply direction decision

				schedule.score += directionDecision.score;
				schedule.steps.push_back(Schedule::FlowRelocationScheduleStep(
					relocationUnit.position,
					directionDecision.fillChange,
					relocationUnit.timeUntilAvailable
				));
			}

			return schedule;
		}
	};
}