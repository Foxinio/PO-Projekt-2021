#pragma once
#include <memory>
#include <chrono>
#include <optional>

#include "Constants.h"
#include "IPerson.h"

class IFloor;
class ICabin;

class IElevatorFacade {
public:
	virtual ~IElevatorFacade() = default;

	virtual void CallElevator(std::unique_ptr<IPerson>&& person, Units::floor floorNumber) = 0;
};


class IElevatorManager : public IElevatorFacade {
public:
	IElevatorManager() = default;
	virtual ~IElevatorManager() = default;
	IElevatorManager(IElevatorManager&&) = default;
	IElevatorManager(const IElevatorManager&) = default;

	IElevatorManager& operator=(IElevatorManager&&) = default;
	IElevatorManager& operator=(const IElevatorManager&) = default;

	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void WaitForStop() = 0;

	virtual void DisablePeopleGeneration() = 0;
	virtual void EnablePeopleGeneration() = 0;

	virtual std::unique_ptr<IPerson> GetCustomer(Units::floor floor, Units::direction cabinDirection) = 0;
	virtual std::optional<const IPerson*> PeekCustomer(Units::floor floor, Units::direction cabinDirection) const = 0;


	// events
	virtual void ElevatorWithoutOrders(const ICabin& caller) = 0;
	virtual void ElevatorArrived(const ICabin& caller) = 0;
	virtual void PeopleLeftCabin(const ICabin& caller) = 0;
};

