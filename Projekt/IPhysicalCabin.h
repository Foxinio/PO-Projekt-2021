#pragma once

#include <optional>

#include "ICabin.h"

class IPhysicalCabin :
    public ICabin
{
private:
	double position;
	double velosity;
	double acceleration;

	double GetAcceleration();

protected:


	virtual inline const double GetMaxVelocity() const noexcept = 0;
	virtual inline const double GetMaxAcceleration() const noexcept = 0;
	virtual std::optional<IFloor::floor> GetCurrentTargetFloor() = 0;

	virtual void PhysicalUpdate(IElevatorManager::deltaTime deltaTime) final;
	
	virtual void ArrivedAtFloor() = 0;
public:
	virtual direction GetDirection();

private:
	void UpdatePosition(IElevatorManager::deltaTime deltaTime);

};

