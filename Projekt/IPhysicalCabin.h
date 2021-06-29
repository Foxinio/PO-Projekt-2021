#pragma once

#include <optional>

#include "ICabin.h"

class IPhysicalCabin :
    public ICabin
{
private:
	double position;
	double velocity;
	double acceleration;

	double GetAcceleration();

protected:

	IPhysicalCabin();
	IPhysicalCabin(Units::floor startingFloor);

	virtual std::optional<Units::floor> GetCurrentTargetFloor() = 0;

	virtual void UpdatePosition(Time::deltaTime deltaTime) final;
	virtual bool HasArrivedAtDestination() final;
	virtual double GetVelocity() final;
	virtual double GetPosition() final;
	virtual double GetMinimalDistance() final;
	
	virtual void ArrivedAtFloor() = 0;
public:
	virtual Units::direction GetDirection();

	virtual ~IPhysicalCabin() = default;

};

