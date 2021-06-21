#include "DefaultPerson.h"

#include <thread>
#include <chrono>


DefaultPerson::DefaultPerson() : 
	weight(80), targerFloor(0), startingFloor(1) { }

DefaultPerson::DefaultPerson(IPerson::weight weight, IFloor::floor startingFloor, IFloor::floor targetFloor) :
	weight(weight), targerFloor(targerFloor), startingFloor(startingFloor) { }

IPerson::weight DefaultPerson::GetWeight() const {
	return weight;
}
IFloor::floor DefaultPerson::GetTarget() const {
	return targerFloor;
}
IFloor::floor DefaultPerson::GetStartingFloor() const {
	return startingFloor;
}

bool DefaultPerson::DoesEnter(ICabin::direction direction) const {
	return true;
}

void DefaultPerson::LeaveCabin() {
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(500ms);
}

void DefaultPerson::EnterCabin() {
	using namespace std::chrono_literals;
	std::this_thread::sleep_for(500ms);
}