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

	static Units::weight maxWeight;
	static std::uint32_t CabinCapacity;

	static double maxAcceleration;
	static double maxVelocity;

	static Time::deltaTime cabinUpdateFrequency;

	static Time::deltaTime GetSimulationTime();

	static std::string TimeToString(Time::deltaTime time);

	static void PrintMessage(std::string sender, std::string message);

	static std::string TagToString(std::uint32_t tag, std::uint32_t fill = 4);

	// Get implementation of ICabin using default constructor
	static std::unique_ptr<ICabin> GetCabin(IElevatorManager& systemManagerReference);

	// Get implementation of ICabin with starting floor
	static std::unique_ptr<ICabin> GetCabin(IElevatorManager& systemManagerReference,
														 Units::floor startingFloor);

	static std::uint32_t GetCabinTag();

	// Get implementation of IPerson using default constructor
	static std::unique_ptr<IPerson> GetPerson();

	// Get implementation of IPerson with given parameters
	static std::unique_ptr<IPerson> GetPerson(Units::weight weight, Units::floor startingFloor, Units::floor targetFloor);

	static std::uint32_t GetPersonTag();

	// Get implementation of IPeopleGenerator with floor range
	static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange);

	static std::unique_ptr<IPeopleGenerator> GetPeopleGenerator(std::pair<Units::floor, Units::floor> floorRange,
																					std::pair<Units::weight, Units::weight> weightRange);

	// Get implementation of IFloor using default constructor
	static std::unique_ptr<IFloor> GetFloor();


	// Get implementation of IElevatorManager using default constructor
	static std::shared_ptr<IElevatorManager> GetElevatorManager(std::size_t numberOfFloors,
																					std::pair<int, int> normalDustributionIntervalParams,
																					Units::floor startingFloor);
};


class Double
{
public:
	static const double accuracy;

	static bool isEqual(const double& lhs, const double& rhs);
};
