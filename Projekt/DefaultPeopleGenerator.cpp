#include "DefaultPeopleGenerator.h"
#include "ObjectFactory.h"

DefaultPeopleGenerator::DefaultPeopleGenerator(std::pair<Units::floor, Units::floor> floors)
	: weightDistribution(50, 90), floorDistribution(floors.first, floors.second), randomEngine((std::uint32_t)ObjectFactory::seed) {	}

DefaultPeopleGenerator::DefaultPeopleGenerator(std::pair<Units::weight, Units::weight> weightRange,
															  std::pair<Units::floor, Units::floor> floors) : 
		weightDistribution(weightRange.first, weightRange.second), 
		floorDistribution(floors.first, floors.second), 
		randomEngine((std::uint32_t)ObjectFactory::seed) {	}

std::unique_ptr<IPerson> DefaultPeopleGenerator::GeneratePerson() {
	auto weight = this->weightDistribution(randomEngine);
	auto startingFloor = this->floorDistribution(randomEngine);
	auto targetFloor = this->floorDistribution(randomEngine);

	while(startingFloor == targetFloor) 
		targetFloor = this->floorDistribution(randomEngine);

	return ObjectFactory::GetPerson(weight, startingFloor, targetFloor);
}