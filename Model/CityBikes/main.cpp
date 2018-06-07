#include "processes/demand-examples.hpp"
#include "processes/distance-examples.hpp"

#include "processes/model-test.hpp"
#include "processes/prediction-test-window-size.hpp"
#include "processes/prediction-test-neighbourhood-size.hpp"
#include "processes/prediction-test-hard.hpp"
#include "processes/prediction-test-hard-avg.hpp"
#include "processes/prediction-test-window-size-without-ongoing.hpp"

#include "processes/redistribution-test.hpp"

#include "processes/demand-prediction-test-weighted.hpp"
#include "processes/demand-prediction-test-not-weighted.hpp"
#include "processes/demand-prediction-test-global.hpp"

#include "processes/flow-test.hpp"

#include "processes/redistribution-test-window-size.hpp"
#include "processes/redistribution-test-configuration.hpp"
#include "processes/redistribution-test-clients.hpp"
#include "processes/redistribution-test-units.hpp"

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
	//CityBikes::Processes::testPredictionWindowSize<Nodes>();
	//CityBikes::Processes::testPredictionNeighbourhoodSize<Nodes>();
	//CityBikes::Processes::testPredictionHard<Nodes>();
	//CityBikes::Processes::testPredictionHardAvg<Nodes>();
	//CityBikes::Processes::testPredictionWindowSizeWithoutOngoing<Nodes>();

	//CityBikes::Processes::testRedistribution<Nodes>(timeFrames);
	//CityBikes::Processes::testRedistributionWindowSize<Nodes>(timeFrames);
	//CityBikes::Processes::testRedistributionConfiguration<Nodes>(timeFrames);
	//CityBikes::Processes::testRedistributionClients<Nodes>(timeFrames);
	CityBikes::Processes::testRedistributionUnits<Nodes>(timeFrames);

	//CityBikes::Processes::testDemandPredictionWeighted<Nodes>(timeFrames);
	//CityBikes::Processes::testDemandPredictionNotWeighted<Nodes>(timeFrames);
	//CityBikes::Processes::testDemandPredictionGlobal<Nodes>(timeFrames);

	//CityBikes::Processes::testFlow<Nodes>(timeFrames, initialStationSize, examplesNumber);

	//CityBikes::Processes::computeFlowExamples<Nodes>(timeFrames);
	//CityBikes::Processes::testStaticAnalysis<Nodes>(timeFrames, examplesNumber)

	return 0;
}