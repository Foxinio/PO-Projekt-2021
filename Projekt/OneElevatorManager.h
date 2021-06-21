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


	void JoinAndDeleteThread(std::thread* t);

	std::unique_ptr<std::thread, decltype(JoinAndDeleteThread)> worker;
	bool working;

	std::mt19937_64 randomEngine;
	std::normal_distribution<double> intervalDirstibution;
	IElevatorManager::timePoint nextCustomerArrivalTimePoint;

public:

	OneElevatorManager();

	OneElevatorManager(std::unique_ptr<ICabin>&& cabin, 
							 std::vector<std::unique_ptr<IFloor>>&& floorVector,
							 std::unique_ptr<IPeopleGenerator>&& customerGenerator,
							 std::pair<int,int> normalDistributionIntervalParams);

	OneElevatorManager(const OneElevatorManager& arg) = delete;
	OneElevatorManager(OneElevatorManager&& arg) = default;

	OneElevatorManager& operator=(const OneElevatorManager& arg) = delete;
	OneElevatorManager& operator=(OneElevatorManager&& arg) = default;

	~OneElevatorManager() = default;

	virtual void Start();
	virtual void Stop();

	virtual void CallElevator(std::unique_ptr<IPerson>&& person, IFloor::floor floorNumber);

	virtual std::unique_ptr<IPerson> GetCustomer(IFloor::floor floor, ICabin::direction cabinDirection);
	virtual std::optional<const IPerson&> PeekCustomer(IFloor::floor floor, ICabin::direction cabinDirection) const noexcept;


	// events
	virtual void ElevatorWithoutOrders(const ICabin& caller);
	virtual void ElevatorArrived(const ICabin& caller);
	virtual void PeopleLeftCabin(const ICabin& caller);

private:
	void Update();
	void GenerateNewCustomer(IElevatorManager::timePoint timePoint);
};

