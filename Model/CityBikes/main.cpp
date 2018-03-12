#include "processes/model-test.hpp"
#include "processes/flow-examples.hpp"
#include "processes/prediction-test.hpp"
#include "processes/flow-test.hpp"
#include "processes/network-fill-test.hpp"
#include "processes/redistribution-test.hpp"

const int stations = 74;

int main()
{
	std::srand(time(NULL));

	const size_t timeFrames = 1440;
	const size_t initialStationSize = 5;
	const size_t examplesNumber = 100;

	//CityBikes::Processes::testModel<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::computeFlowExamples<stations>(timeFrames);
	//CityBikes::Processes::testPrediction<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testFlow<stations>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testNetworkFill<stations>(timeFrames, initialStationSize, examplesNumber);
	CityBikes::Processes::testRedistribution<stations>(timeFrames, initialStationSize, examplesNumber);

	return 0;
}