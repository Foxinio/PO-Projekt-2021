#pragma once
#include <vector>
#include <memory>

#include "Constants.h"

class ICabin {
public:

	virtual void Update(Time::timePoint time) = 0;

	virtual void CallCabin(Units::floor floor) = 0;
	virtual Units::direction GetDirection() = 0;
};

