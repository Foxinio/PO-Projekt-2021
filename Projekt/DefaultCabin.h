#pragma once

#include <list>
#include <queue>
#include <mutex>

#include "IPhysicalCabin.h"

class DefaultCabin :
	public IPhysicalCabin
{
	IPerson::weight onBoardWeight = 0;
	std::list<std::unique_ptr<IPerson>> passangers;
	
	std::priority_queue<IFloor::floor> destinationQueue;
	std::mutex destinationQueueMutex;

	std::shared_ptr<IElevatorManager> systemManager;

	IElevatorManager::timePoint lastUpdateTimePoint;


public:
	DefaultCabin(std::shared_ptr<IElevatorManager> systemManager, IFloor::floor startingFloor = 0);

	virtual void Update(IElevatorManager::timePoint time) override;

	virtual void CallCabin(IFloor::floor floor) override;

private:

	virtual inline const double GetMaxVelocity() const noexcept;
	virtual inline const double GetMaxAcceleration() const noexcept;
	virtual std::optional<IFloor::floor> GetCurrentTargetFloor();

	virtual void ArrivedAtFloor() override;

	void EnterCabin(std::unique_ptr<IPerson>&& customer);
	void EmptyCabin(IFloor::floor currentFloor);
	void FillCabin(IFloor::floor currentFloor);

	bool priority_queueLess(const IFloor::floor& lhs, const IFloor::floor& rhs);
};

