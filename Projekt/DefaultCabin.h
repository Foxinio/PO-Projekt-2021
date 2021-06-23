#pragma once

#include <list>
#include <queue>
#include <mutex>

#include "IPhysicalCabin.h"
#include "ObjectFactory.h"
#include "FloorQueue.h"

class DefaultCabin :
	public IPhysicalCabin
{
	Units::weight onBoardWeight = 0;
	std::list<std::unique_ptr<IPerson>> passangers;

	FloorQueue destinationQueue;
	std::mutex destinationQueueMutex;

	std::shared_ptr<IElevatorManager> systemManager;

	Time::timePoint lastUpdateTimePoint;

	Time::timePoint lastLogUpdate;
	Tag tag;

public:
	DefaultCabin(std::shared_ptr<IElevatorManager> systemManager, Units::floor startingFloor);
	DefaultCabin(std::shared_ptr<IElevatorManager> systemManager);

	virtual void Update(Time::timePoint time) override;

	virtual void CallCabin(Units::floor floor) override;

private:

	virtual std::optional<Units::floor> GetCurrentTargetFloor();

	virtual void ArrivedAtFloor() override;

	void EnterCabin(std::unique_ptr<IPerson>&& customer);
	void EmptyCabin(Units::floor currentFloor);
	void FillCabin(Units::floor currentFloor);

	bool priority_queueLess(const Units::floor& lhs, const Units::floor& rhs);
};

