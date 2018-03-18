#pragma once

#include "../../CityBikes.Data/flow/flow-instance.hpp"
#include "../../CityBikes.Data/time/day.hpp"

#include <vector>
#include <map>

namespace CityBikes::DataProcessing::Flow
{
	class Filtering
	{
	public:
		static std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> getOthers(
			const std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> examples,
			Data::Time::Day day
		)
		{
			std::map<Data::Time::Day, std::vector<Data::Flow::FlowInstance>> otherExamples;
			for (auto& otherExample : examples)
				if (otherExample.first != day)
					otherExamples.emplace(otherExample.first, otherExample.second);

			return otherExamples;
		}
	};
}