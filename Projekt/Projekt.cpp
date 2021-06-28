#include <iostream>
#include <thread>
#include <chrono>

#include "ObjectFactory.h"
#include "IElevatorManager.h"

int main()
{
	using namespace std::chrono_literals;
	// gkt init here

	// begin simulation
	ObjectFactory::seed = 0x38275234;
	ObjectFactory::CabinCapacity = 10;
	ObjectFactory::maxWeight = 1600;

	ObjectFactory::maxVelocity = 5;
	ObjectFactory::maxAcceleration = 2;

	ObjectFactory::cabinUpdateFrequency = 2s;

	auto manager = ObjectFactory::GetElevatorManager(8, { 10, 5 }, 4);
	manager->Start();
	

	// work in infinite loop until commanded to stop
	std::this_thread::sleep_for(15s);

	// stop simulation
	manager->DisablePeopleGeneration();
	manager->WaitForStop();
}
