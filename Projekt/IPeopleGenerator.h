#pragma once
#include <memory>

#include "IPerson.h"

class IPeopleGenerator
{
public:
	virtual std::unique_ptr<IPerson> GeneratePerson() = 0;
};

