#include <sstream>
#include <iomanip>
#include <iostream>

#include "ObjectFactory.h"


#include "DefaultCabin.h"
#include "DefaultFloor.h"
#include "DefaultPeopleGenerator.h"
#include "DefaultPerson.h"
#include "OneElevatorManager.h"

std::uint64_t ObjectFactory::seed = 0;

const double Double::accuracy = 1.0e-8;

Units::weight ObjectFactory::maxWeight = 1600;
std::uint32_t ObjectFactory::CabinCapacity = 8;

double ObjectFactory::maxAcceleration = 2.0;
double ObjectFactory::maxVelocity = 4;

Time::deltaTime ObjectFactory::cabinUpdateFrequency = std::chrono::seconds(5);


Time::deltaTime ObjectFactory::GetSimulationTime() {
	static Time::timePoint beginTime = Time::clock::now();
	return Time::clock::now() - beginTime;
}

std::string ObjectFactory::TimeToString(Time::deltaTime time) {

	std::uint32_t minutes = (std::uint32_t)std::chrono::duration_cast<std::chrono::minutes>(time).count();
	std::uint32_t seconds = (std::uint32_t)std::chrono::duration_cast<std::chrono::seconds>(time).count() % 60;
	std::uint32_t milliseconds = (std::uint32_t)std::chrono::duration_cast<std::chrono::milliseconds>(time).count() % 1000;

	std::stringstream ss;
	ss << std::setfill('0') << std::setw(2) << minutes;
	ss << ":" << std::setfill('0') << std::setw(2) << seconds;
	ss << "." << std::setfill('0') << std::setw(3) << milliseconds;

	return ss.str();
}

std::string ObjectFactory::TagToString(std::uint32_t tag, std::uint32_t fill) {
	std::stringstream ss;
	ss << std::setfill('0') << std::setw(fill) << tag;
	return ss.str();
}

static std::mutex clogMutex{};
void ObjectFactory::PrintMessage(std::string sender, std::string message) {
	using namespace std::string_literals;
	std::string output = "["s + ObjectFactory::TimeToString(ObjectFactory::GetSimulationTime()) + "] " + sender + ":\t" + message + "\n";
	std::unique_lock<std::mutex> lock{ clogMutex };
	std::clog << output;
}

// Get implementation of ICabin using default constructor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(IElevatorManager& systemManagerReference) {
	return std::make_unique<DefaultCabin>(systemManagerReference);
}

// Get implementation of ICabin with starting floor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(IElevatorManager& systemManagerReference, Units::floor startingFloor) {
	return std::make_unique<DefaultCabin>(systemManagerReference, startingFloor);
}

static std::mutex cabinTagMutex{};
static std::uint32_t cabinTagCounter = 0;
std::uint32_t ObjectFactory::GetCabinTag() {
	std::unique_lock<std::mutex> lock{ cabinTagMutex};
	return cabinTagCounter++;
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

static std::mutex personTagMutex{};
static std::uint32_t personTagCounter = 0;
std::uint32_t ObjectFactory::GetPersonTag() {
	std::unique_lock<std::mutex> lock{ personTagMutex };
	return personTagCounter++;
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
std::shared_ptr<IElevatorManager> ObjectFactory::GetElevatorManager(std::size_t numberOfFloors, std::pair<int,int> normalDustributionIntervalParams, Units::floor startingFloor) {
	std::vector<std::unique_ptr<IFloor>> floors;
	for (std::uint32_t i = 0; i < numberOfFloors; ++i) {
		floors.push_back(ObjectFactory::GetFloor());
	}
	auto generator = GetPeopleGenerator({ 0, numberOfFloors - 1 });


	return std::make_shared<OneElevatorManager>(std::move(floors),
															  std::move(generator),
															  normalDustributionIntervalParams,
															  startingFloor);
}


bool Double::isEqual(const double& lhs, const double& rhs) {
	return std::abs(lhs - rhs) < accuracy;
}