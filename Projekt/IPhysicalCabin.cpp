#include "IPhysicalCabin.h"
#include "ObjectFactory.h"

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
	double abs = (double)GetCurrentTargetFloor().value() - position;
	if (abs < 0) {
		return Units::direction::Down;
	}
	else {
		return Units::direction::Up;
	}
}

double IPhysicalCabin::GetAcceleration() {
	double atLeastDistance = velocity * velocity / ObjectFactory::maxAcceleration;
	if(GetDirection() == Units::direction::Up) {
		return ObjectFactory::maxAcceleration * (this->position + atLeastDistance >= GetCurrentTargetFloor() ? -1 : 1);
	}
	else {
		return ObjectFactory::maxAcceleration * (this->position - atLeastDistance <= GetCurrentTargetFloor() ? 1 : -1);
	}
}