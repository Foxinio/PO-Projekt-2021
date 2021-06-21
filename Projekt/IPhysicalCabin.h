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


	virtual std::optional<Units::floor> GetCurrentTargetFloor() = 0;

	virtual void UpdatePosition(Time::deltaTime deltaTime) final;
	virtual bool HasArrivedAtDestination() final;
	
	virtual void ArrivedAtFloor() = 0;
public:
	virtual Units::direction GetDirection();

private:

};

