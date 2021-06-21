#pragma once

#include "Constants.h"

class IPerson {
public:

	virtual Units::weight GetWeight() const = 0;
	virtual Units::floor GetTarget() const = 0;
	virtual Units::floor GetStartingFloor() const = 0;

	virtual bool DoesEnter(Units::direction direction) const = 0;

	virtual void LeaveCabin() = 0;
	virtual void EnterCabin() = 0;
};

