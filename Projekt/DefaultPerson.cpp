#include "DefaultPerson.h"

#include <thread>
#include <chrono>


DefaultPerson::DefaultPerson() : 
	weight(80), targerFloor(0), startingFloor(1) { }

DefaultPerson::DefaultPerson(Units::weight weight, Units::floor startingFloor, Units::floor targetFloor) :
	weight(weight), targerFloor(targerFloor), startingFloor(startingFloor) { }

Units::weight DefaultPerson::GetWeight() const {
	return weight;
}
Units::floor DefaultPerson::GetTarget() const {
	return targerFloor;
}
Units::floor DefaultPerson::GetStartingFloor() const {
	return startingFloor;
}

bool DefaultPerson::DoesEnter(Units::direction direction) const {
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