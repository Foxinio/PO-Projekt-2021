#pragma once
#include "IPeopleGenerator.h"
#include <random>

class DefaultPeopleGenerator :
    public IPeopleGenerator
{
	std::mt19937 g;
	std::uniform_int_distribution<IPerson::weight> weightDistribution;
	std::uniform_int_distribution<IFloor::floor> floorDistribution;

public:
	DefaultPeopleGenerator(std::pair<IFloor::floor, IFloor::floor> floors);
	DefaultPeopleGenerator(std::pair<IFloor::floor, IFloor::floor> floors,
								  std::pair<IPerson::weight, IPerson::weight> weightRange);

	virtual std::unique_ptr<IPerson> GeneratePerson();
};

