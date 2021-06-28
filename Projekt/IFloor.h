#pragma once
#include <memory>

#include "ICabin.h"
#include "IPerson.h"

class IFloor
{
public:
	virtual ~IFloor() = default;

	virtual void JoinQueue(std::unique_ptr<IPerson>&& person) = 0;
	virtual std::unique_ptr<IPerson> GetCustomer(Units::direction cabinDirection) = 0;
	virtual std::optional<const IPerson*> PeekCustomer(Units::direction cabinDirection) const = 0;
};

