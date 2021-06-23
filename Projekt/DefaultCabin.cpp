#include "DefaultCabin.h"
#include "ObjectFactory.h"

#include <cassert>
#include <sstream>
#include <iostream>
#include <iomanip>

std::optional<Units::floor> DefaultCabin::GetCurrentTargetFloor() {
	if (destinationQueue.empty()) {
		return std::nullopt;
	}
	return std::optional<Units::floor>{ this->destinationQueue.top() };
}

DefaultCabin::DefaultCabin(std::shared_ptr<IElevatorManager> systemManager, Units::floor startingFloor) :
		onBoardWeight(0),
		passangers(),
		destinationQueue(),
		destinationQueueMutex(),
		systemManager(systemManager),
		lastUpdateTimePoint(Time::clock::now()),
		tag(ObjectFactory::GetCabinTag()),
		lastLogUpdate(Time::clock::now()) { }

void DefaultCabin::ArrivedAtFloor() {
	systemManager->ElevatorArrived(*this);

	std::unique_lock<std::mutex> lock(destinationQueueMutex);
	assert(!destinationQueue.empty());
	auto currentFloor = GetCurrentTargetFloor().value();
	this->destinationQueue.pop();
	
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag, 1), "Cabin arrived on floor "s + std::to_string(currentFloor) + ". Begining emptying.");

	lock.unlock();


	systemManager->ElevatorArrived(*this);

	EmptyCabin(currentFloor);
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag, 1), "Cabin emptied. Begining filling.");
	FillCabin(currentFloor);
	ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag, 1), "Cabin filled.");
}

void DefaultCabin::EnterCabin(std::unique_ptr<IPerson>&& customer) {
	customer->EnterCabin();
	CallCabin(customer->GetTarget());
	passangers.push_back(std::move(customer));
}

void DefaultCabin::EmptyCabin(Units::floor currentFloor) {
	auto iter = passangers.begin();
	while (iter != passangers.end()) {
		if ((*iter)->GetTarget() == currentFloor) {
			auto temp = iter;
			++iter;
			(*temp)->LeaveCabin();
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
	}

	if (IPhysicalCabin::HasArrivedAtDestination()) {
		ArrivedAtFloor();
	}

	using namespace std::string_literals;
	using namespace std::chrono_literals;
	if (lastLogUpdate + ObjectFactory::cabinUpdateFrequency < Time::clock::now()) {
		auto message = (std::stringstream() << "Update: position " << std::setprecision(3) << std::fixed << GetPosition()
			<< "floor, velocity " << GetVelocity() << "floors/s.").str();

		ObjectFactory::PrintMessage("Cabin#"s + ObjectFactory::TagToString(tag, 1), message);
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
	auto& ref = Container(destinationQueue);
	if(std::find(ref.begin(), ref.end(), floor) == ref.end())
		destinationQueue.push(floor);
}

bool DefaultCabin::priority_queueLess(const Units::floor& pushing, const Units::floor& inPlace) {
	auto currentTarget = GetCurrentTargetFloor();

	if (!currentTarget.has_value()) {
		return true;
	}

	auto dir = GetDirection();

	if (dir == Units::direction::Up) {
		if (pushing > currentTarget.value()) {
			return pushing < inPlace;
		}
		else {
			return pushing >= inPlace;
		}
	}
	else {
		if (pushing > currentTarget.value()) {
			return pushing >= inPlace;
		}
		else {
			return pushing < inPlace;
		}
	}
}