#pragma once

#include "IElevatorManager.h"
#include "ObjectFactory.h"

#include <functional>
#include <thread>
#include <random>

class OneElevatorManager :
    public IElevatorManager
{
   std::unique_ptr<ICabin> cabin;
	std::unique_ptr<IPeopleGenerator> customerGenerator;
   std::vector<std::unique_ptr<IFloor>> floors;



	std::thread worker;
	bool working;

	bool generatePeople;
	std::mt19937_64 randomEngine;
	std::normal_distribution<double> intervalDirstibution;
	Time::timePoint nextCustomerArrivalTimePoint;

public:

	OneElevatorManager();

	OneElevatorManager(std::vector<std::unique_ptr<IFloor>>&& floorVector,
							 std::unique_ptr<IPeopleGenerator>&& customerGenerator,
							 std::pair<int,int> normalDistributionIntervalParams,
							 Units::floor startingFloor);

	OneElevatorManager(const OneElevatorManager& arg) = delete;
	OneElevatorManager(OneElevatorManager&& arg) = default;

	OneElevatorManager& operator=(const OneElevatorManager& arg) = delete;
	OneElevatorManager& operator=(OneElevatorManager&& arg) = default;

	~OneElevatorManager();

	virtual void Start();
	virtual void Stop();
	virtual void WaitForStop();

	virtual void DisablePeopleGeneration();
	virtual void EnablePeopleGeneration();

	virtual void CallElevator(std::unique_ptr<IPerson>&& person, Units::floor floorNumber);

	virtual std::unique_ptr<IPerson> GetCustomer(Units::floor floor, Units::direction cabinDirection);
	virtual std::optional<const IPerson*> PeekCustomer(Units::floor floor, Units::direction cabinDirection) const;


	// events
	virtual void ElevatorWithoutOrders(const ICabin& caller);
	virtual void ElevatorArrived(const ICabin& caller);
	virtual void PeopleLeftCabin(const ICabin& caller);

private:
	void Update();
	void GenerateNewCustomer(Time::timePoint timePoint);
};

