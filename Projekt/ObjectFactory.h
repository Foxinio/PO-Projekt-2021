#pragma once
#include <memory>

#include "IElevatorManager.h"
#include "IPeopleGenerator.h"
#include "ICabin.h"
#include "IFloor.h"
#include "IPerson.h"

#include "Constants.h"


class ObjectFactory
{
public:

	static std::uint64_t seed;

	static const Units::weight maxWeight;

	static const double maxAcceleration;
	static const double maxVelocity;

	static Time::deltaTime GetSimulationTime();

	static std::string TimeToString(Time::deltaTime time);

	// Get implementation of ICabin using default constructor
	static std::unique_ptr<ICabin> GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference);

	// Get implementation of ICabin with starting floor
	static std::unique_ptr<ICabin> GetCabin(std::shared_ptr<IElevatorManager> systemManagerReference,
														 Units::floor startingFloor);

	// Get implementation of IPerson using default constructor
	static std::unique_ptr<IPerson> GetPerson();

	// Get implementation of IPerson with given parameters
	static std::unique_ptr<IPerson> GetPerson(Units::weight weight, Units::floor startingFloor, Units::floor targetFloor);

	// Get implementation of IPeopleGenerator with floor range
	static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange);

	static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange,
																					std::pair<Units::weight, Units::weight> weightRange);

	// Get implementation of IFloor using default constructor
	static std::unique_ptr<IFloor> GetFloor();


	// Get implementation of IElevatorManager using default constructor
	static std::shared_ptr<IElevatorManager> GetElevatorManager(std::size_t numberOfFloors,
																					std::pair<int, int> normalDustributionIntervalParams);
};


class Double
{
public:
	static const double accuracy;

	static bool isEqual(const double& lhs, const double& rhs);
};
