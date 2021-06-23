#pragma once
#include <list>
#include <functional>

#include "Constants.h"

class FloorQueue
{
	std::list<Units::floor> container;
	std::function<bool(Units::floor, Units::floor)> comp;
public:
	FloorQueue(std::function<bool(Units::floor, Units::floor)> comp = std::less<Units::floor>());

	bool is_empty();
	void push(Units::floor floor);
	void pop();
	Units::floor top();

	std::list<Units::floor>::const_iterator cbegin();
	std::list<Units::floor>::const_iterator cend();
};

