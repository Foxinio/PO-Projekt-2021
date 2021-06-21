#include "ObjectFactory.h"


#include "DefaultCabin.h"
#include "DefaultFloor.h"
#include "DefaultPeopleGenerator.h"
#include "DefaultPerson.h"
#include "OneElevatorManager.h"

std::uint64_t ObjectFactory::seed = 0;

const double Double::accuracy = 1.0e-8;

// Get implementation of ICabin using default constructor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference) {
	return std::make_unique<DefaultCabin>(systemManagerReference);
}

// Get implementation of ICabin with starting floor
std::unique_ptr<ICabin> ObjectFactory::GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference, IFloor::floor startingFloor) {
	return std::make_unique<DefaultCabin>(systemManagerReference, startingFloor);
}

// Get implementation of IPerson using default constructor
std::unique_ptr<IPerson> ObjectFactory::GetPerson() {
	return std::make_unique<DefaultPerson>();
}

// Get implementation of IPerson with given parameters
std::unique_ptr<IPerson> ObjectFactory::GetPerson(IPerson::weight weight, 
																  IFloor::floor startingFloor, 
																  IFloor::floor targetFloor) {
	return std::make_unique<DefaultPerson>(weight, startingFloor, targetFloor);
}

// Get implementation of IPeopleGenerator with floor range
static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<IFloor::floor, IFloor::floor> floorRange) {
	return std::make_unique<DefaultPeopleGenerator>(floorRange);
}

// Get implementation of IPeopleGenerator with floor range
static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<IFloor::floor, IFloor::floor> floorRange,
																				std::pair<IPerson::weight, IPerson::weight> weightRange) {
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

	std::vector<std::unique_ptr<IFloor>> floors = { numberOfFloors, nullptr };
	std::generate(std::begin(floors), std::end(floors), [](){	return std::move(ObjectFactory::GetFloor()); });

	auto generator = GetPeopleGenerator({ 0, numberOfFloors - 1 });


	*result = OneElevatorManager(std::move(cabin),
										  std::move(floors),
										  std::move(generator),
										  normalDustributionIntervalParams);
	return result;
}