#include "DefaultCabin.h"
#include "ObjectFactory.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <iomanip>

std::optional<Units::floor> DefaultCabin::GetCurrentTargetFloor() {
	if (destinationQueue.is_empty()) {
		return std::nullopt;
	}
	return std::optional<Units::floor>{ this->destinationQueue.top() };
}

DefaultCabin::DefaultCabin(std::shared_ptr<IElevatorManager> systemManager, Units::floor startingFloor) :
		IPhysicalCabin(startingFloor),
		passangers(),
		destinationQueue([&](Units::floor lhs, Units::floor rhs) {	return priority_queueLess(lhs, rhs); }),
		destinationQueueMutex(),
		systemManager(systemManager),
		lastUpdateTimePoint(Time::clock::now()),
		lastLogUpdate(Time::clock::now()),
		onBoardWeight(0),
		tag(ObjectFactory::GetCabinTag()) { }

DefaultCabin::DefaultCabin(std::shared_ptr<IElevatorManager> systemManager) :
		DefaultCabin(systemManager, 0) { }

void DefaultCabin::ArrivedAtFloor() {
	systemManager->ElevatorArrived(*this);

	std::unique_lock<std::mutex> lock(destinationQueueMutex);
	assert(!destinationQueue.is_empty());
	auto currentFloor = GetCurrentTargetFloor().value();
	this->destinationQueue.pop();
	
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag), "Cabin arrived on floor "s + std::to_string(currentFloor) + ". Begining emptying.");

	lock.unlock();


	systemManager->ElevatorArrived(*this);

	EmptyCabin(currentFloor);
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag), "Cabin emptied. Begining filling.");
	FillCabin(currentFloor);
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag), "Cabin filled.");
}

void DefaultCabin::EnterCabin(std::unique_ptr<IPerson>&& customer) {
	customer->EnterCabin(tag);
	CallCabin(customer->GetTarget());
	passangers.push_back(std::move(customer));
}

void DefaultCabin::EmptyCabin(Units::floor currentFloor) {
	auto iter = passangers.begin();
	while (iter != passangers.end()) {
		if ((*iter)->GetTarget() == currentFloor) {
			auto temp = iter;
			++iter;
			(*temp)->LeaveCabin(tag);
			onBoardWeight -= (*temp)->GetWeight();
			passangers.erase(temp, iter);
		}
		else {
			++iter;
		}
	}
	systemManager->PeopleLeftCabin(*this);
}
void DefaultCabin::FillCabin(Units::floor currentFloor) {
	while (true) {
		auto nextCustomer = systemManager->PeekCustomer(currentFloor, IPhysicalCabin::GetDirection());

		if (!nextCustomer.has_value()) {
			break;
		}

		if (onBoardWeight + nextCustomer.value()->GetWeight() < ObjectFactory::maxWeight &&
			 nextCustomer.value()->DoesEnter(IPhysicalCabin::GetDirection()) &&
			 this->passangers.size() < ObjectFactory::CabinCapacity) {
			onBoardWeight += nextCustomer.value()->GetWeight();
			EnterCabin(std::move(systemManager->GetCustomer(currentFloor, IPhysicalCabin::GetDirection())));
		}
		else {
			break;
		}
	}
}

void DefaultCabin::Update(Time::timePoint time) {
	if(GetCurrentTargetFloor().has_value()) {
		auto deltaTime = time - lastUpdateTimePoint;
		IPhysicalCabin::UpdatePosition(deltaTime);
	}
	else {
		systemManager->ElevatorWithoutOrders(*this);
		if (!GetCurrentTargetFloor().has_value()) return;
	}

	if (IPhysicalCabin::HasArrivedAtDestination()) {
		ArrivedAtFloor();
	}

	if (lastLogUpdate + ObjectFactory::cabinUpdateFrequency < Time::clock::now()) {
		using namespace std::string_literals;
		using namespace std::chrono_literals;
		std::stringstream ss;
		ss << "Update: position " << std::setprecision(3) << std::fixed << GetPosition()
			<< "floor, velocity " << GetVelocity() << "floors/s";
		auto message = ss.str();

		if (auto target = GetCurrentTargetFloor(); target) {
			message += ", target "s + std::to_string(target.value()) + ".";
		}
		else {
			message += ".";
		}

		ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag), message);
		lastLogUpdate = Time::clock::now();
	}
	lastUpdateTimePoint = Time::clock::now();
}

template<class T, class S, class C>
struct HackedQueue : private std::priority_queue<T, S, C> {
	static S& Container(std::priority_queue<T, S, C>& q) {
		return q.* & HackedQueue::c;
	}
};
template <class T, class S, class C>
S& Container(std::priority_queue<T, S, C>& q) {
	return HackedQueue<T, S, C>::Container(q);
}


void DefaultCabin::CallCabin(Units::floor floor) {
	std::unique_lock<std::mutex> lock(destinationQueueMutex);
	destinationQueue.push(floor);
	// For Debuging, may come useful later
	// 
	//std::stringstream ss;
	//ss << "\t";
	//std::copy(destinationQueue.cbegin(), destinationQueue.cend(), std::ostream_iterator<Units::floor>(ss, " > "));
	//ss << "\n";
	//std::cout << ss.str();
}

bool DefaultCabin::priority_queueLess(const Units::floor& pushing, const Units::floor& inPlace) {
	auto currentTarget = GetCurrentTargetFloor();
	auto dir = GetDirection();

	// For Debuging, may come useful later
	// 
	//std::stringstream ss;
	//ss << "\tcomparing elements for the queue: " << pushing << " <=> " << inPlace
	//	<< ".\n\tcurrent target: " << currentTarget.value()
	//	<< ".\n\tcurrent direction: " << (dir == Units::direction::Up ? "Up" : dir == Units::direction::Down ? "Down" : "NoDir") << ".\n";
	//std::cout << ss.str();

	if (inPlace == currentTarget) {
		if (dir == Units::direction::Up) {
			return pushing < inPlace && GetPosition() + GetMinimalDistance() < pushing;
		}
		else {
			return pushing > inPlace && GetPosition() + GetMinimalDistance() > pushing;
		}
	}

	// inPlace = 3, pushing = 4, currentTarget = 7
	if (dir == Units::direction::Up) {
		if (inPlace < currentTarget) {
			return pushing > currentTarget || pushing > inPlace;
		}
		else { // inPlace > currentTarget
			return pushing > currentTarget && pushing < inPlace;
		}
	}
	else { // dir == Units::direction::Down
		if (inPlace > currentTarget) {
			return pushing < currentTarget || pushing < inPlace;
		}
		else { // inPlace < currentTarget
			return pushing < currentTarget && pushing > inPlace;
		}
	}
}