#pragma once
#include <random>

#include "Constants.h"
#include "IPeopleGenerator.h"

class DefaultPeopleGenerator :
    public IPeopleGenerator
{
	std::mt19937 randomEngine;
	std::uniform_int_distribution<Units::weight> weightDistribution;
	std::uniform_int_distribution<Units::floor> floorDistribution;

public:
	DefaultPeopleGenerator(std::pair<Units::floor, Units::floor> floors);
	DefaultPeopleGenerator(std::pair<Units::floor, Units::floor> floors,
								  std::pair<Units::weight, Units::weight> weightRange);

	virtual std::unique_ptr<IPerson> GeneratePerson();
};

