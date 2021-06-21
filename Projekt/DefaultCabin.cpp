#include "DefaultCabin.h"
#include "ObjectFactory.h"

#include <cassert>

inline const double DefaultCabin::GetMaxVelocity() const noexcept {
	return std::numeric_limits<double>::max();
}
inline const double DefaultCabin::GetMaxAcceleration() const noexcept {
	return 2.0;
}
std::optional<IFloor::floor> DefaultCabin::GetCurrentTargetFloor() {
	if (destinationQueue.empty()) {
		return std::nullopt;
	}
	return this->destinationQueue.top();
}

DefaultCabin::DefaultCabin(std::shared_ptr<IElevatorManager> systemManager, IFloor::floor startingFloor) :
		onBoardWeight(0),
		passangers(),
		destinationQueue(),
		destinationQueueMutex(),
		systemManager(systemManager),
		lastUpdateTimePoint(IElevatorManager::clock::now()) { }

void DefaultCabin::ArrivedAtFloor() {
	systemManager->ElevatorArrived(*this);

	std::unique_lock<std::mutex> lock(destinationQueueMutex);
	assert(!destinationQueue.empty());
	auto currentFloor = GetCurrentTargetFloor().value();
	this->destinationQueue.pop();
	
	lock.unlock();

	systemManager->ElevatorArrived(*this);

	EmptyCabin(currentFloor);
	FillCabin(currentFloor);
}

void DefaultCabin::EnterCabin(std::unique_ptr<IPerson>&& customer) {
	customer->EnterCabin();
	CallCabin(customer->GetTarget());
	passangers.push_back(std::move(customer));
}

void DefaultCabin::EmptyCabin(IFloor::floor currentFloor) {
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
void DefaultCabin::FillCabin(IFloor::floor currentFloor) {
	while (true) {
		auto nextCustomer = systemManager->PeekCustomer(currentFloor, IPhysicalCabin::GetDirection());

		if (!nextCustomer) {
			break;
		}

		if (onBoardWeight + nextCustomer.value().GetWeight() < ObjectFactory::maxWeight &&
			 nextCustomer.value().DoesEnter(IPhysicalCabin::GetDirection())) {
			EnterCabin(std::move(systemManager->GetCustomer(currentFloor, IPhysicalCabin::GetDirection())));
		}
		else {
			break;
		}
	}
}

void DefaultCabin::Update(IElevatorManager::timePoint time) {
	if(GetCurrentTargetFloor()) {
		auto deltaTime = time - lastUpdateTimePoint;
		IPhysicalCabin::PhysicalUpdate(deltaTime);
	}
	else {
		systemManager->ElevatorWithoutOrders(*this);
	}
	lastUpdateTimePoint = IElevatorManager::clock::now();
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


void DefaultCabin::CallCabin(IFloor::floor floor) {
	std::unique_lock<std::mutex> lock(destinationQueueMutex);
	auto& ref = Container(destinationQueue);
	if(std::find(ref.begin(), ref.end(), floor) == ref.end())
		destinationQueue.push(floor);
}

bool DefaultCabin::priority_queueLess(const IFloor::floor& pushing, const IFloor::floor& inPlace) {
	auto currentTarget = GetCurrentTargetFloor();

	if (!currentTarget) {
		return true;
	}

	auto dir = GetDirection();

	if (dir == ICabin::direction::Up) {
		if (pushing > currentTarget) {
			return pushing < inPlace;
		}
		else {
			return pushing >= inPlace;
		}
	}
	else {
		if (pushing > currentTarget) {
			return pushing >= inPlace;
		}
		else {
			return pushing < inPlace;
		}
	}
}