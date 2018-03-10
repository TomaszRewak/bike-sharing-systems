#include "processes/model-test.hpp"
#include "processes/flow-examples.hpp"
#include "processes/prediction-test.hpp"
#include "processes/flow-test.hpp"
#include "processes/network-fill-test.hpp"

const int stations = 74;

int main()
{
	const size_t timeFrames = 1440;
	const size_t initialStationSize = 5;
	const size_t examplesNumber = 1;

	//CityBikes::Processes::testModel<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::computeFlowExamples<stations>(timeFrames);
	CityBikes::Processes::testPrediction<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testFlow<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testNetworkFill<stations>(timeFrames, initialStationSize, examplesNumber);

	return 0;
}