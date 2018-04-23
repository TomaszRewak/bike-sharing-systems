#include "processes/demand-examples.hpp"
#include "processes/distance-examples.hpp"

#include "processes/model-test.hpp"
#include "processes/prediction-test.hpp"
#include "processes/redistribution-test.hpp"

#include "processes/experiment.hpp"
#include "processes/flow-test.hpp"

//#include "processes/flow-examples.hpp"
//#include "processes/static-analysis-test.hpp"

const int Nodes = 74;

int main()
{
	std::srand(time(NULL));

	const size_t timeFrames = 144;
	const size_t initialStationSize = 5;
	const size_t examplesNumber = 100;


	//CityBikes::Processes::computeDemandExamples<Nodes>();
	//CityBikes::Processes::computeDistanceExamples<Nodes>();

	//CityBikes::Processes::testModel<Nodes>(timeFrames, initialStationSize, examplesNumber);
	//CityBikes::Processes::testPrediction<Nodes>(timeFrames);
	//CityBikes::Processes::testRedistribution<Nodes>(timeFrames);

	CityBikes::Processes::experiment<Nodes>(timeFrames);
	//CityBikes::Processes::testFlow<Nodes>(timeFrames, initialStationSize, examplesNumber);

	//CityBikes::Processes::computeFlowExamples<Nodes>(timeFrames);
	//CityBikes::Processes::testStaticAnalysis<Nodes>(timeFrames, examplesNumber)

	return 0;
}