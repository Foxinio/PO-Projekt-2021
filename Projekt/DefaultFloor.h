#pragma once
#include <memory>
#include <queue>
#include <optional>

#include "IFloor.h"

class DefaultFloor :
    public IFloor
{
	std::queue<std::unique_ptr<IPerson>> queue;

public:
	DefaultFloor();

	virtual ~DefaultFloor() = default;

	virtual void JoinQueue(std::unique_ptr<IPerson>&& person);
	virtual std::unique_ptr<IPerson> GetCustomer(Units::direction cabinDirection);
	virtual std::optional<const IPerson*> PeekCustomer(Units::direction cabinDirection) const;
};

