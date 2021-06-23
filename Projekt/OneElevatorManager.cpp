#include "OneElevatorManager.h"

#include <cassert>
#include <iostream>



OneElevatorManager::OneElevatorManager() : 
		cabin(nullptr), 
		floors(), 
		customerGenerator(nullptr),
		intervalDirstibution(),
		nextCustomerArrivalTimePoint(Time::clock::now()),
		randomEngine(ObjectFactory::seed),
		worker(),
		working(false) { }

OneElevatorManager::OneElevatorManager(std::vector<std::unique_ptr<IFloor>>&& floorVector,
													std::unique_ptr<IPeopleGenerator>&& customerGenerator,
													std::pair<int, int> normalDistributionIntervalParams) :
		cabin(ObjectFactory::GetCabin(std::shared_ptr<OneElevatorManager>(this))),
		floors(),
		customerGenerator(std::move(customerGenerator)),
		intervalDirstibution((double)normalDistributionIntervalParams.first, (double)normalDistributionIntervalParams.second),
		nextCustomerArrivalTimePoint(Time::clock::now()),
		randomEngine(ObjectFactory::seed),
		worker(),
		working(false) {
	std::move(std::begin(floorVector), std::end(floorVector), std::back_inserter(floors));
}

//
//OneElevatorManager::OneElevatorManager(OneElevatorManager&& arg) :
//		cabin(std::move(arg.cabin)),
//		floors(std::move(arg.floors)),
//		customerGenerator(std::move(arg.customerGenerator)),
//		intervalDirstibution(std::move(arg.intervalDirstibution)),
//		nextCustomerArrivalTimePoint(std::chrono::time_point_cast<Time::timePoint::duration>(clock::now())),
//		randomEngine(std::move(arg.randomEngine)),
//		worker(std::move(arg.worker)),
//		working(arg.working) { }
//
//
//OneElevatorManager& OneElevatorManager::operator=(OneElevatorManager&& arg) {
//	cabin = std::move(arg.cabin);
//	floors = std::move(arg.floors);
//	customerGenerator = std::move(arg.customerGenerator);
//	intervalDirstibution = std::move(arg.intervalDirstibution);
//	nextCustomerArrivalTimePoint = std::chrono::time_point_cast<Time::timePoint::duration>(clock::now());
//	randomEngine = std::move(std::mt19937_64(ObjectFactory::seed));
//	worker = std::move(arg.worker);
//	working = arg.working;
//
//	arg.working = false;
//}

OneElevatorManager::~OneElevatorManager() {
	Stop();
}

void OneElevatorManager::Start() {
	assert(cabin != nullptr);
	assert(customerGenerator != nullptr);
	working = true;
	worker = std::thread{ [this]() {
			Update();
		} 
	};
}
void OneElevatorManager::Stop() {
	working = false;
	if(worker.joinable()) worker.join();
}

void OneElevatorManager::CallElevator(std::unique_ptr<IPerson>&& person, Units::floor floorNumber) {
	cabin->CallCabin(floorNumber);
	floors[floorNumber]->JoinQueue(std::move(person));
}

std::unique_ptr<IPerson> OneElevatorManager::GetCustomer(Units::floor floor, Units::direction cabinDirection) {
	return std::move(floors[floor]->GetCustomer(cabinDirection));
}
std::optional<const IPerson*> OneElevatorManager::PeekCustomer(Units::floor floor, Units::direction cabinDirection) const {
	return floors[floor]->PeekCustomer(cabinDirection);
}


// events
void OneElevatorManager::ElevatorWithoutOrders(const ICabin& caller) {
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

		if (this->nextCustomerArrivalTimePoint < timePoint) {
			GenerateNewCustomer(timePoint);
		}

		cabin->Update(timePoint);
	}
	ObjectFactory::PrintMessage("SystemMenager", "Simulation Ended.");
}

void OneElevatorManager::GenerateNewCustomer(Time::timePoint timePoint) {
	auto customer = customerGenerator->GeneratePerson();
	auto floor = customer->GetStartingFloor();
	CallElevator(std::move(customer), floor);

	// must multiply by 100 because intervalDistribution returns distribution in seconds
	Time::deltaTime interval = std::chrono::milliseconds((long)(intervalDirstibution(randomEngine) * 1000));
	nextCustomerArrivalTimePoint = Time::clock::now() + interval;
}
