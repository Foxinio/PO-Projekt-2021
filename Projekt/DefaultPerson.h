#pragma once

#include "IPerson.h"


class DefaultPerson :
    public IPerson
{
	Units::weight weight;
	Units::floor targerFloor;
	Units::floor startingFloor;
	Tag tag;

public:
	DefaultPerson();
	DefaultPerson(Units::weight weight, Units::floor startingFloor, Units::floor targetFloor);

	virtual Units::weight GetWeight() const;
	virtual Units::floor GetTarget() const;
	virtual Units::floor GetStartingFloor() const;

	virtual bool DoesEnter(Units::direction direction) const;

	virtual void LeaveCabin(Tag cabinTag) override;
	virtual void EnterCabin(Tag cabinTag) override;
};

