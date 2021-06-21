#pragma once
#include <vector>
#include <memory>

#include "IFloor.h"
#include "IPerson.h"
#include "IElevatorManager.h"

class ICabin {
public:
	enum class direction {
		Up,
		Down
	};

	virtual void Update(IElevatorManager::timePoint time) = 0;

	virtual void CallCabin(IFloor::floor floor) = 0;
	virtual direction GetDirection() = 0;
};

