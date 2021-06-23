#include "DefaultPerson.h"
#include "ObjectFactory.h"

#include <thread>
#include <chrono>


DefaultPerson::DefaultPerson() : 
		weight(80),
		targerFloor(0),
		startingFloor(1),
		tag(ObjectFactory::GetPersonTag()) {
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Person#"s + ObjectFactory::TagToString(tag), "New Person aproaching elevator on floor "s + std::to_string(startingFloor) +
		", target floor " + std::to_string(targerFloor) + ".");
}

DefaultPerson::DefaultPerson(Units::weight weight, Units::floor startingFloor, Units::floor targetFloor) :
		weight(weight), 
		targerFloor(targetFloor),
		startingFloor(startingFloor), 
		tag(ObjectFactory::GetPersonTag()) {
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Person#"s + ObjectFactory::TagToString(tag), "New Person aproaching elevator on floor "s + std::to_string(startingFloor) +
		", target floor " + std::to_string(targerFloor) + ".");
}

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

void DefaultPerson::LeaveCabin(Tag cabinTag) {
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Person#"s + ObjectFactory::TagToString(tag), "Leaving Cabin#" + ObjectFactory::TagToString(cabinTag) + " on floor "s + std::to_string(targerFloor));
	std::this_thread::sleep_for(500ms);
}

void DefaultPerson::EnterCabin(Tag cabinTag) {
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	ObjectFactory::PrintMessage("Person#"s + ObjectFactory::TagToString(tag), "Entering Cabin#" + ObjectFactory::TagToString(cabinTag) + " on floor "s + std::to_string(startingFloor));
	std::this_thread::sleep_for(500ms);
}