#include <iostream>
#include <thread>
#include <chrono>

#include "ObjectFactory.h"
#include "IElevatorManager.h"

int main()
{
	// gkt init here
	bool continueWorking = true;

	// begin simulation
	auto manager = ObjectFactory::GetElevatorManager();
	manager->Init();
	

	// work in infinite loop until commanded to stop
	using namespace std::chrono_literals;
	while (continueWorking) std::this_thread::sleep_for(500ms);

	// stop simulation
	manager->Stop();
}
