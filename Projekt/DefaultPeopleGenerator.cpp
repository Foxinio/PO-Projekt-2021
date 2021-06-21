#include "DefaultPeopleGenerator.h"
#include "ObjectFactory.h"

DefaultPeopleGenerator::DefaultPeopleGenerator(std::pair<IFloor::floor, IFloor::floor> floors)
	: weightDistribution(50, 90), floorDistribution(floors.first, floors.second), g(ObjectFactory::seed) {	}

DefaultPeopleGenerator::DefaultPeopleGenerator(std::pair<IPerson::weight, IPerson::weight> weightRange,
															  std::pair<IFloor::floor, IFloor::floor> floors) : 
		weightDistribution(weightRange.first, weightRange.second), 
		floorDistribution(floors.first, floors.second), 
		g(ObjectFactory::seed) {	}

std::unique_ptr<IPerson> DefaultPeopleGenerator::GeneratePerson() {
	auto weight = this->weightDistribution(g);
	auto startingFloor = this->floorDistribution(g);
	auto targetFloor = this->floorDistribution(g);

	while(startingFloor == targetFloor) 
		targetFloor = this->floorDistribution(g);

	return ObjectFactory::GetPerson(weight, startingFloor, targetFloor);
}