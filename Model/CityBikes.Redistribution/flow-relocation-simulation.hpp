#pragma once

#include "../CityBikes.Scheduling/flow-relocation-scheduler.hpp"
#include "../CityBikes.Model/flow-distribution-simulation.hpp"
#include "../CityBikes.Model/flow-prediction-simulation.hpp"
#include "../CityBikes.DataProcessing/flow/casting.hpp"

namespace CityBikes::Redistribution
{
	template<size_t Nodes>
	class FlowRelocationSimulation
	{
	private:
		size_t predictionWindow;

		double baseOperationTime;
		double singleOperationTime;

		int minThresholdValue;
		int maxThresholdValue;

		size_t scheduleLength;
		size_t scheduleExamples;


	public:
		FlowRelocationSimulation(
			size_t predictionWindow,
			double baseOperationTime, double singleOperationTime,
			int minThresholdValue, int maxThresholdValue,
			size_t scheduleLength, size_t scheduleExamples
		) :
			predictionWindow(predictionWindow),
			baseOperationTime(baseOperationTime),
			singleOperationTime(singleOperationTime),
			minThresholdValue(minThresholdValue),
			maxThresholdValue(maxThresholdValue),
			scheduleLength(scheduleLength),
			scheduleExamples(scheduleExamples)
		{ }

		Data::FillLevel::FillLevelPrediction<Nodes> simulate(
			Model::FlowDistributionSimulation<Nodes> flowDistributionSimulation,
			const Data::Supply::SupplyPrediction<Nodes>& ongoingSupplyPrediction,
			const Data::Demand::DemandPrediction<Nodes>& upcomingDemandPrediction,
			const Data::Supply::SupplyPrediction<Nodes>& upcomingSupplyPrediction,
			Data::Relocation::RelocationTeam relocationTeam,
			const Data::FlowTime::FlowTimePrediction<Nodes>& flowTimePrediction,
			size_t timeFrames
		)
		{
			Data::FillLevel::FillLevelPrediction<Nodes> result;

			while (flowDistributionSimulation.getTimeFrame() < timeFrames)
			{
				auto& currentState = flowDistributionSimulation.getCurrentState();
				auto ongoingFlow = DataProcessing::Flow::Casting::getOngoingFlow(flowDistributionSimulation.getOngoingInstances());

				size_t startTimeFrame = flowDistributionSimulation.getTimeFrame();
				size_t endTimeFrame = std::min(startTimeFrame + predictionWindow, timeFrames);

				Data::FillLevel::FillLevelPrediction<Nodes> prediction = Model::FlowPredictionSimulation<Nodes>::predict(
					currentState,
					ongoingFlow,
					ongoingSupplyPrediction,
					upcomingDemandPrediction,
					upcomingSupplyPrediction,
					startTimeFrame, endTimeFrame
				);
				prediction.insert(prediction.begin(), currentState);

				Scheduling::Decisions::DecisionApplier<Nodes> decisionApplier(
					baseOperationTime, singleOperationTime,
					flowTimePrediction, startTimeFrame
				);

				Scheduling::FlowRelocationScheduler<Nodes> scheduler(
					decisionApplier,
					minThresholdValue, maxThresholdValue,
					scheduleLength, scheduleExamples
				);

				std::vector<Data::Relocation::RelocationSchedule> schedules = scheduler.schedule(
					relocationTeam,
					prediction
				);

				for (size_t unit = 0; unit < relocationTeam.size(); unit++)
				{
					auto& relocationUnit = relocationTeam[unit];
					auto& schedule = schedules[unit];

					for (auto& operation : schedule)
					{
						if (relocationUnit.timeUntilDestination >= 1)
							break;

						decisionApplier.check(relocationUnit, currentState, operation);
						decisionApplier.apply(relocationUnit, currentState, operation);
					}

					relocationUnit.timeUntilDestination = std::max(0., relocationUnit.timeUntilDestination - 1);
				}

				result.push_back(flowDistributionSimulation.getCurrentState());

				flowDistributionSimulation.step();
			}

			return result;
		}
	};

}