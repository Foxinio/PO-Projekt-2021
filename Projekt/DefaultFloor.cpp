#include <iostream>

#include "DefaultFloor.h"

DefaultFloor::DefaultFloor() : queue() { }

void DefaultFloor::JoinQueue(std::unique_ptr<IPerson>&& person) {
	queue.push(std::move(person));
}

std::unique_ptr<IPerson> DefaultFloor::GetCustomer(Units::direction cabinDirection) {
	auto result = std::move(queue.front());
	queue.pop();
	return result;
}

std::optional<const IPerson*> DefaultFloor::PeekCustomer(Units::direction cabinDirection) const {
	if (queue.empty()) {
		return std::nullopt;
	}
	return std::optional<const IPerson*>{ queue.front().get() };
}