#include "FloorQueue.h"



FloorQueue::FloorQueue(std::function<bool(Units::floor, Units::floor)> comp) : container(), comp(comp) { }

bool FloorQueue::is_empty() {
	return container.empty();
}

void FloorQueue::push(Units::floor floor) {
	if (container.size() < 1) {
		container.push_back(floor);
	}
	else {
		for (auto iter = std::begin(container); true; ++iter) {
			if (iter == std::end(container)) {
				container.push_back(floor);
				break;
			}
			if (floor == *iter) break;
			if (comp(floor, *iter)) {
				container.insert(iter, floor);
				break;
			}
		}
	}
}

void FloorQueue::pop() {
	container.pop_front();
}

Units::floor FloorQueue::top() {
	return container.front();
}


std::list<Units::floor>::const_iterator FloorQueue::cbegin() {
	return container.cbegin();
}

std::list<Units::floor>::const_iterator FloorQueue::cend() {
	return container.cend();
}

