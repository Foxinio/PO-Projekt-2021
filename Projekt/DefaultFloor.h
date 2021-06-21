#pragma once
#include <memory>
#include <queue>

#include "IFloor.h"

class DefaultFloor :
    public IFloor
{
	std::queue<std::unique_ptr<IPerson>> queue;

public:
	DefaultFloor();

	virtual void JoinQueue(std::unique_ptr<IPerson>&& person);
	virtual std::unique_ptr<IPerson> GetCustomer(ICabin::direction cabinDirection);
	virtual std::optional<const IPerson&> PeekCustomer(ICabin::direction cabinDirection) const;
};

