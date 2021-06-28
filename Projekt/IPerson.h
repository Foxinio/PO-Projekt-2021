#pragma once

#include "Constants.h"

class IPerson {
public:
	virtual ~IPerson() = default;

	virtual Units::weight GetWeight() const = 0;
	virtual Units::floor GetTarget() const = 0;
	virtual Units::floor GetStartingFloor() const = 0;

	virtual bool DoesEnter(Units::direction direction) const = 0;

	virtual void LeaveCabin(Tag cabinTag) = 0;
	virtual void EnterCabin(Tag cabinTag) = 0;
};

