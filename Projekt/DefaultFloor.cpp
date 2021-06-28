#include <iostream>

#include "DefaultFloor.h"

DefaultFloor::DefaultFloor() : queue() { }

void DefaultFloor::JoinQueue(std::unique_ptr<IPerson>&& person) {
	queue.push(std::move(person));
}

std::unique_ptr<IPerson> DefaultFloor::GetCustomer(Units::direction cabinDirection) {
	if (queue.front()->DoesEnter(cabinDirection)) {
		auto result = std::move(queue.front());
		queue.pop();
		return result;
	}
	else {
		return nullptr;
	}
}

std::optional<const IPerson*> DefaultFloor::PeekCustomer(Units::direction cabinDirection) const {
	if (queue.empty()) {
		return std::nullopt;
	}
	return std::optional<const IPerson*>{ queue.front().get() };
}