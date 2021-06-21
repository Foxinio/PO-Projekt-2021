#pragma once
#include "IPerson.h"
class DefaultPerson :
    public IPerson
{
	IPerson::weight weight;
	IFloor::floor targerFloor;
	IFloor::floor startingFloor;
public:
	DefaultPerson();
	DefaultPerson(IPerson::weight weight, IFloor::floor startingFloor, IFloor::floor targetFloor);

	virtual IPerson::weight GetWeight() const;
	virtual IFloor::floor GetTarget() const;
	virtual IFloor::floor GetStartingFloor() const;

	virtual bool DoesEnter(ICabin::direction direction) const;

	virtual void LeaveCabin() override;
	virtual void EnterCabin() override;
};

