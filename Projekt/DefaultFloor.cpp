#include "DefaultFloor.h"

DefaultFloor::DefaultFloor() : queue() { }

void DefaultFloor::JoinQueue(std::unique_ptr<IPerson>&& person) {
	queue.push(std::move(person));
}

std::unique_ptr<IPerson> DefaultFloor::GetCustomer(ICabin::direction cabinDirection) {
	auto result = std::move(queue.front());
	queue.pop();
	return result;
}

std::optional<const IPerson&> DefaultFloor::PeekCustomer(ICabin::direction cabinDirection) const {
	if (queue.empty()) {
		return std::nullopt;
	}
	return *queue.front();
}