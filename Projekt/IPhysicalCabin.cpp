#include "IPhysicalCabin.h"
#include "ObjectFactory.h"

IPhysicalCabin::IPhysicalCabin(Units::floor startingFloor) :
	position(startingFloor),
	velocity(0),
	acceleration(0) { }

double IPhysicalCabin::GetPosition() {
	return position;
}

double IPhysicalCabin::GetVelocity() {
	return velocity;
}

void IPhysicalCabin::UpdatePosition(Time::deltaTime deltaTime) {
	acceleration = GetAcceleration();
	// After cast time is in seconds
	velocity += acceleration * std::chrono::duration_cast<std::chrono::duration<double>>(deltaTime).count();
	if (std::abs(velocity) > ObjectFactory::maxVelocity) {
		velocity = ObjectFactory::maxVelocity * (velocity < 0 ? -1 : 1);
	}
	// After cast time is in seconds
	position += velocity * std::chrono::duration_cast<std::chrono::duration<double>>(deltaTime).count();
}

bool IPhysicalCabin::HasArrivedAtDestination() {
	return Double::isEqual(position, GetCurrentTargetFloor().value());
}

Units::direction IPhysicalCabin::GetDirection() {
	if (auto dir = GetCurrentTargetFloor(); dir) {
		double abs = (double)dir.value() - position;
		if (abs < 0) {
			return Units::direction::Down;
		}
		else {
			return Units::direction::Up;
		}
	}
	else {
		return Units::direction::NoDir;
	}
}

double IPhysicalCabin::GetMinimalDistance() {
	return (velocity < 0 ? -1 : 1) * velocity * velocity / ObjectFactory::maxAcceleration;
}

double IPhysicalCabin::GetAcceleration() {
	double atLeastDistance = GetMinimalDistance();
	if (GetDirection() == Units::direction::Up) {
		return ObjectFactory::maxAcceleration * (this->position + atLeastDistance >= GetCurrentTargetFloor() ? -1 : 1);
	}
	else if (GetDirection() == Units::direction::Down) {
		return ObjectFactory::maxAcceleration * (this->position + atLeastDistance <= GetCurrentTargetFloor() ? 1 : -1);
	}
	else {
		return 0.0;
	}
}