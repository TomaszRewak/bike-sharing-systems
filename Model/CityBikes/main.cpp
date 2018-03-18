#include "processes/model-test.hpp"
#include "processes/prediction-test.hpp"
#include "processes/redistribution-test.hpp"

#include "processes/flow-test.hpp"

//#include "processes/flow-examples.hpp"
//#include "processes/static-analysis-test.hpp"
//#include "processes/demand-examples.hpp"

const int Nodes = 74;

int main()
{
	std::srand(time(NULL));

	const size_t timeFrames = 288;
	const size_t initialStationSize = 5;
	const size_t examplesNumber = 100;

	//CityBikes::Processes::testModel<Nodes>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testPrediction<Nodes>(timeFrames);
	CityBikes::Processes::testRedistribution<Nodes>(timeFrames);

	//CityBikes::Processes::testFlow<Nodes>(timeFrames, initialStationSize, examplesNumber);

	//CityBikes::Processes::computeFlowExamples<Nodes>(timeFrames);
	//CityBikes::Processes::testStaticAnalysis<Nodes>(timeFrames, examplesNumber)
	//CityBikes::Processes::computeDemandExamples<Nodes>();

	return 0;
}