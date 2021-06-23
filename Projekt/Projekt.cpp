#include <iostream>
#include <thread>
#include <chrono>

#include "ObjectFactory.h"
#include "IElevatorManager.h"

int main()
{
	// gkt init here

	// begin simulation
	auto manager = ObjectFactory::GetElevatorManager(8, { 10, 5 }, 4);
	manager->Start();
	

	// work in infinite loop until commanded to stop
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(1min);

	// stop simulation
	manager->DisablePeopleGeneration();
	manager->WaitForStop();
}
