#include <sstream>
#include <iomanip>

#include "ObjectFactory.h"


#include "DefaultCabin.h"
#include "DefaultFloor.h"
#include "DefaultPeopleGenerator.h"
#include "DefaultPerson.h"
#include "OneElevatorManager.h"

std::uint64_t ObjectFactory::seed = 0;

const double Double::accuracy = 1.0e-8;

const Units::weight ObjectFactory::maxWeight = 1600;

const double ObjectFactory::maxAcceleration = 1.6;
const double ObjectFactory::maxVelocity = 4;

Time::deltaTime ObjectFactory::GetSimulationTime() {
	static Time::timePoint beginTime = Time::clock::now();
	return Time::clock::now() - beginTime;
}

std::string ObjectFactory::TimeToString(Time::deltaTime time) {

	std::uint32_t minutes = std::chrono::duration_cast<std::chrono::minutes>(time).count();
	std::uint32_t seconds = std::chrono::duration_cast<std::chrono::seconds>(time).count() % 60;
	std::uint32_t miliseconds = (std::chrono::duration_cast<std::chrono::milliseconds>(time).count() % 1000) / 10;

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << minutes;
	ss << std::setfill('0') << std::setw(2) << ":" << seconds;
	ss << std::setfill('0') << std::setw(3) << "." << seconds;

	return ss.str();
}

// Get implementation of ICabin using default constructor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference) {
	return std::make_unique<DefaultCabin>(systemManagerReference);
}

// Get implementation of ICabin with starting floor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference, Units::floor startingFloor) {
	return std::make_unique<DefaultCabin>(systemManagerReference, startingFloor);
}

// Get implementation of IPerson using default constructor
std::unique_ptr<IPerson> ObjectFactory::GetPerson() {
	return std::make_unique<DefaultPerson>();
}

// Get implementation of IPerson with given parameters
std::unique_ptr<IPerson> ObjectFactory::GetPerson(Units::weight weight, 
																  Units::floor startingFloor, 
																  Units::floor targetFloor) {
	return std::make_unique<DefaultPerson>(weight, startingFloor, targetFloor);
}

// Get implementation of IPeopleGenerator with floor range
std::unique_ptr<IPeopleGenerator> ObjectFactory::GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange) {
	return std::make_unique<DefaultPeopleGenerator>(floorRange);
}

// Get implementation of IPeopleGenerator with floor range
std::unique_ptr<IPeopleGenerator> ObjectFactory::GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange,
																						  std::pair<Units::weight, Units::weight> weightRange) {
	return std::make_unique<DefaultPeopleGenerator>(floorRange, weightRange);
}

// Get implementation of IFloor using default constructor
std::unique_ptr<IFloor> ObjectFactory::GetFloor() {
	return std::make_unique<DefaultFloor>();
}

// Get implementation of IElevatorManager using default constructor
std::shared_ptr<IElevatorManager> ObjectFactory::GetElevatorManager(std::size_t numberOfFloors, std::pair<int,int> normalDustributionIntervalParams) {
	std::shared_ptr<OneElevatorManager> result = std::make_shared<OneElevatorManager>();
	
	auto cabin = GetCabin(result);

	std::vector<std::unique_ptr<IFloor>> floors;
	//std::vector<std::unique_ptr<IFloor>> floors = { numberOfFloors, nullptr };
	//std::generate(std::begin(floors), std::end(floors), [](){	return std::move(ObjectFactory::GetFloor()); });
	for (int i = 0; i < numberOfFloors; ++i) {
		floors.push_back(ObjectFactory::GetFloor());
	}
	auto generator = GetPeopleGenerator({ 0, numberOfFloors - 1 });


	*result = std::move(OneElevatorManager(std::move(cabin),
														std::move(floors),
														std::move(generator),
														normalDustributionIntervalParams));
	return result;
}


bool Double::isEqual(const double& lhs, const double& rhs) {
	return std::abs(lhs - rhs) < accuracy;
}