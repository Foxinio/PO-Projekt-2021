#pragma once
#include <memory>

class IPerson;

class IPeopleGenerator
{
public:
	virtual std::unique_ptr<IPerson> GeneratePerson() = 0;

	virtual ~IPeopleGenerator() = default;
};

