#include "OneElevatorManager.h"

#include <cassert>
#include <iostream>



OneElevatorManager::OneElevatorManager() : 
		OneElevatorManager(std::vector<std::unique_ptr<IFloor>>(), nullptr, { 0,0 }, 0) { 
	cabin = nullptr;
}

OneElevatorManager::OneElevatorManager(std::vector<std::unique_ptr<IFloor>>&& floorVector,
													std::unique_ptr<IPeopleGenerator>&& customerGenerator,
													std::pair<int, int> normalDistributionIntervalParams,
													Units::floor startingFloor) :
		cabin(ObjectFactory::GetCabin(std::shared_ptr<OneElevatorManager>(this), startingFloor)),
		customerGenerator(std::move(customerGenerator)),
		floors(),
		worker(),
		randomEngine(ObjectFactory::seed),
		intervalDirstibution((double)normalDistributionIntervalParams.first, (double)normalDistributionIntervalParams.second),
		nextCustomerArrivalTimePoint(Time::clock::now()),
		working(false),
		generatePeople(true) {
	std::move(std::begin(floorVector), std::end(floorVector), std::back_inserter(floors));
}

OneElevatorManager::~OneElevatorManager() {
	working = false;
	if (worker.joinable()) worker.join();
}

void OneElevatorManager::Start() {
	assert(cabin != nullptr);
	assert(customerGenerator != nullptr);
	working = true;
	ObjectFactory::PrintMessage("SystemManager", "Simulation started.");
	worker = std::thread{ [this]() {
			Update();
		} 
	};
}
void OneElevatorManager::Stop() {
	working = false;
	ObjectFactory::PrintMessage("SystemManager", "Simulation force stopped.");
	if(worker.joinable()) worker.join();
}
void OneElevatorManager::WaitForStop() {
	if(worker.joinable()) worker.join();
}

void OneElevatorManager::DisablePeopleGeneration() {
	generatePeople = false;
	ObjectFactory::PrintMessage("SystemManager", "People generation disabled.");
}
void OneElevatorManager::EnablePeopleGeneration() {
	ObjectFactory::PrintMessage("SystemManager", "People generation enabled.");
	generatePeople = true;
}

void OneElevatorManager::CallElevator(std::unique_ptr<IPerson>&& person, Units::floor floorNumber) {
	cabin->CallCabin(floorNumber);
	floors[(size_t)floorNumber]->JoinQueue(std::move(person));
}

std::unique_ptr<IPerson> OneElevatorManager::GetCustomer(Units::floor floor, Units::direction cabinDirection) {
	return std::move(floors[(size_t)floor]->GetCustomer(cabinDirection));
}
std::optional<const IPerson*> OneElevatorManager::PeekCustomer(Units::floor floor, Units::direction cabinDirection) const {
	return floors[(size_t)floor]->PeekCustomer(cabinDirection);
}


// events
void OneElevatorManager::ElevatorWithoutOrders(const ICabin& caller) {
	ObjectFactory::PrintMessage("SystemManager", "Elevator without orders. Stoping Simulation.");
	working = false;
	return;
}
void OneElevatorManager::ElevatorArrived(const ICabin& caller) {
	return;
}
void OneElevatorManager::PeopleLeftCabin(const ICabin& caller) {
	return;
}


void OneElevatorManager::Update() {
	while (working) {
		auto timePoint = Time::clock::now();

		if (generatePeople && this->nextCustomerArrivalTimePoint <= timePoint) {
			GenerateNewCustomer(timePoint);
		}

		cabin->Update(timePoint);
	}
	ObjectFactory::PrintMessage("SystemManager", "Simulation Ended.");
}

void OneElevatorManager::GenerateNewCustomer(Time::timePoint timePoint) {
	auto customer = customerGenerator->GeneratePerson();
	auto floor = customer->GetStartingFloor();
	CallElevator(std::move(customer), floor);

	// must multiply by 1000 because intervalDistribution returns distribution in seconds
	Time::deltaTime interval = std::chrono::milliseconds((long)(intervalDirstibution(randomEngine) * 1000));
	nextCustomerArrivalTimePoint = timePoint + interval;
}
