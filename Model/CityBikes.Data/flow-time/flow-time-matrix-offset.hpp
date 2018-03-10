#pragma once

#include "flow-time-matrix.hpp"

namespace CityBikes::Data::FlowTime
{
	template<size_t Nodes>
	class FlowMatrixOffset
	{
	private:
		const FlowTimeMatrix<Nodes> & flowMatrix;
		size_t timeOffset;

	public:
		FlowMatrixOffset(const FlowTimeMatrix<Nodes>& flowMatrix, size_t timeOffset) :
			flowMatrix(flowMatrix),
			timeOffset(timeOffset)
		{ }

		size_t flowDuration(size_t source, size_t destination, size_t timeFrame) const
		{
			timeFrame += timeOffset;

			return flowMatrix.flowDuration(source, destination, timeFrame);
		}
	};
}