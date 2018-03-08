#pragma once

#include "flow-matrix.hpp"

namespace CityBikes::Data::Flow
{
	class FlowMatrixOffset
	{
	private:
		const FlowMatrix & flowMatrix;
		size_t timeOffset;

	public:
		FlowMatrixOffset(const FlowMatrix& flowMatrix, size_t timeOffset) :
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