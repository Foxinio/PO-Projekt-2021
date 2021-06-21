#pragma once
#include "IFloor.h"
#include "ICabin.h"

class IPerson {
public:
	using weight = int;

	virtual weight GetWeight() const = 0;
	virtual IFloor::floor GetTarget() const = 0;
	virtual IFloor::floor GetStartingFloor() const = 0;

	virtual bool DoesEnter(ICabin::direction direction) const = 0;

	virtual void LeaveCabin() = 0;
	virtual void EnterCabin() = 0;
};

