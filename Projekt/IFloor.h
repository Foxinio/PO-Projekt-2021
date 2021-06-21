#pragma once
#include <memory>

#include "IPerson.h"

class IFloor
{
public:
	using floor = int;

	virtual void JoinQueue(std::unique_ptr<IPerson>&& person) = 0;
	virtual std::unique_ptr<IPerson> GetCustomer(ICabin::direction cabinDirection) = 0;
	virtual std::optional<const IPerson&> PeekCustomer(ICabin::direction cabinDirection) const = 0;
};

