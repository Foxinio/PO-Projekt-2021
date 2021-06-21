#include "OneElevatorManager.h"

#include <cassert>
#include <iostream>



OneElevatorManager::OneElevatorManager() : 
		cabin(nullptr), 
		floors(), 
		customerGenerator(nullptr),
		intervalDirstibution(),
		nextCustomerArrivalTimePoint(std::chrono::time_point_cast<IElevatorManager::timePoint::duration>(clock::now())),
		randomEngine(ObjectFactory::seed),
		worker(nullptr, JoinAndDeleteThread),
		working(false) { }

OneElevatorManager::OneElevatorManager(std::unique_ptr<ICabin>&& cabin,
													std::vector<std::unique_ptr<IFloor>>&& floorVector,
													std::unique_ptr<IPeopleGenerator>&& customerGenerator,
													std::pair<int, int> normalDistributionIntervalParams) :
		cabin(std::move(cabin)),
		floors(std::move(floorVector)),
		customerGenerator(std::move(customerGenerator)),
		intervalDirstibution((double)normalDistributionIntervalParams.first/1000, (double)normalDistributionIntervalParams.second/1000),
		nextCustomerArrivalTimePoint(std::chrono::time_point_cast<IElevatorManager::timePoint::duration>(clock::now())),
		randomEngine(ObjectFactory::seed),
		worker(nullptr, JoinAndDeleteThread),
		working(false) {
	assert(cabin != nullptr);
	assert(customerGenerator != nullptr);
}

//
//OneElevatorManager::OneElevatorManager(OneElevatorManager&& arg) :
//		cabin(std::move(arg.cabin)),
//		floors(std::move(arg.floors)),
//		customerGenerator(std::move(arg.customerGenerator)),
//		intervalDirstibution(std::move(arg.intervalDirstibution)),
//		nextCustomerArrivalTimePoint(std::chrono::time_point_cast<IElevatorManager::timePoint::duration>(clock::now())),
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
//	nextCustomerArrivalTimePoint = std::chrono::time_point_cast<IElevatorManager::timePoint::duration>(clock::now());
//	randomEngine = std::move(std::mt19937_64(ObjectFactory::seed));
//	worker = std::move(arg.worker);
//	working = arg.working;
//
//	arg.working = false;
//}

void OneElevatorManager::Start() {
	working = true;
	worker = { new std::thread{&Update}, JoinAndDeleteThread };
}
void OneElevatorManager::Stop() {
	working = false;
	worker->join();
}

void OneElevatorManager::CallElevator(std::unique_ptr<IPerson>&& person, IFloor::floor floorNumber) {
	cabin->CallCabin(floorNumber);
	floors[floorNumber]->JoinQueue(std::move(person));
}

std::unique_ptr<IPerson> OneElevatorManager::GetCustomer(IFloor::floor floor, ICabin::direction cabinDirection) {
	return std::move(floors[floor]->GetCustomer(cabinDirection));
}
std::optional<const IPerson&> OneElevatorManager::PeekCustomer(IFloor::floor floor, ICabin::direction cabinDirection) const noexcept {
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
		auto timePoint = IElevatorManager::clock::now();

		if (this->nextCustomerArrivalTimePoint < timePoint) {
			GenerateNewCustomer(timePoint);
		}

		cabin->Update(timePoint);
	}
	std::clog << "Simulation Ended";
}

void OneElevatorManager::JoinAndDeleteThread(std::thread* t) {
	working = false;
	if(t->joinable()) t->join();
	delete t;
}

void OneElevatorManager::GenerateNewCustomer(IElevatorManager::timePoint timePoint) {
	auto customer = customerGenerator->GeneratePerson();
	auto floor = customer->GetStartingFloor();
	CallElevator(std::move(customer), floor);

	// must multiply by 100 because intervalDistribution returns distribution in seconds
	IElevatorManager::deltaTime interval = IElevatorManager::deltaTime((long)(intervalDirstibution(randomEngine) * 1000));
	nextCustomerArrivalTimePoint = IElevatorManager::clock::now() + interval;
}
