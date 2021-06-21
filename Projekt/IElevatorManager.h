#pragma once
#include <memory>
#include <chrono>
#include <optional>

#include "IPerson.h"
#include "IFloor.h"
#include "ICabin.h"

class IElevatorFacade {
	public:

		virtual void CallElevator(std::unique_ptr<IPerson>&& person, IFloor::floor floorNumber) = 0;
};


class IElevatorManager : public IElevatorFacade
{
	public:
		using deltaTime = std::chrono::milliseconds;
		using timePoint = std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds>;
		class clock : public std::chrono::high_resolution_clock {
			using internalClock = std::chrono::high_resolution_clock;
		public:
			static inline timePoint now() { 
				return std::chrono::time_point_cast<IElevatorManager::timePoint::duration>(internalClock::now()); 
			}
		};

		virtual void Start() = 0;
		virtual void Stop() = 0;


		virtual std::unique_ptr<IPerson> GetCustomer(IFloor::floor floor, ICabin::direction cabinDirection) = 0;
		virtual std::optional<const IPerson&> PeekCustomer(IFloor::floor floor, ICabin::direction cabinDirection) const noexcept = 0;


		// events
		virtual void ElevatorWithoutOrders(const ICabin& caller) = 0;
		virtual void ElevatorArrived(const ICabin& caller) = 0;
		virtual void PeopleLeftCabin(const ICabin& caller) = 0;
};

