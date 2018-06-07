#pragma once

#include <map>


#include "../CityBikes.Data/fill-level/fill-level-prediction.hpp"
#include "../CityBikes.Data/flow/flow-action.hpp"
#include "../CityBikes.DataProcessing/flow/sorting.hpp"

namespace CityBikes::Model
{
	template<size_t Nodes>
	class FlowDistributionSimulation
	{
	private:
		size_t timeFrame = 0;
		size_t unhandledCustomers = 0;

		Data::FillLevel::FillLevelPredictionFrame<Nodes> currentState;

		std::vector<Data::Flow::FlowInstance> upcomingInstances;
		std::vector<Data::Flow::FlowInstance> ongoingInstances;

	public:
		FlowDistributionSimulation(
			Data::FillLevel::FillLevelPredictionFrame<Nodes> initialState,
			std::vector<Data::Flow::FlowInstance> instances
		) :
			currentState(initialState),
			upcomingInstances(instances)
		{
			DataProcessing::Flow::Sorting::sortInstancesDescendingByStartTime(upcomingInstances);
		}

		void step()
		{
			// perform actions

			while (upcomingInstances.size() && upcomingInstances.back().source.timeFrame == timeFrame)
			{
				Data::Flow::FlowInstance instance = upcomingInstances.back();
				upcomingInstances.pop_back();

				if (currentState[instance.source.node] >= 1)
				{
					currentState[instance.source.node]--;

					ongoingInstances.push_back(instance);
				}
				else
				{
					unhandledCustomers++;
				}
			}

			// apply pending returns

			DataProcessing::Flow::Sorting::sortInstancesDescendingByEndTime(ongoingInstances);

			while (ongoingInstances.size() && ongoingInstances.back().destination.timeFrame <= timeFrame)
			{
				Data::Flow::FlowInstance instance = ongoingInstances.back();
				ongoingInstances.pop_back();

				currentState[instance.destination.node]++;
			}

			// update model

			timeFrame++;
		}

		Data::FillLevel::FillLevelPrediction<Nodes> run(size_t timeFrames)
		{
			Data::FillLevel::FillLevelPrediction<Nodes> prediction;

			while (timeFrame < timeFrames)
			{
				step();
				prediction.push_back(currentState);
			}

			return prediction;
		}

		size_t getTimeFrame() { return timeFrame; }
		size_t getUnhandledCustomers() { return unhandledCustomers; }
		Data::FillLevel::FillLevelPredictionFrame<Nodes>& getCurrentState() { return currentState; }
		const std::vector<Data::Flow::FlowInstance>& getUpcomingInstances() { return upcomingInstances; }
		const std::vector<Data::Flow::FlowInstance>& getOngoingInstances() { return ongoingInstances; }
	};
}