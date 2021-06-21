#include "IPhysicalCabin.h"
#include "ObjectFactory.h"

void IPhysicalCabin::UpdatePosition(IElevatorManager::deltaTime deltaTime) {
	acceleration = GetAcceleration();
	velosity += acceleration * std::chrono::duration_cast<std::chrono::duration<double>>(deltaTime).count();
	position += velosity * std::chrono::duration_cast<std::chrono::duration<double>>(deltaTime).count();
}

void IPhysicalCabin::PhysicalUpdate(IElevatorManager::deltaTime deltaTime) {

	UpdatePosition(deltaTime);

	if (Double::isEqual(position, GetCurrentTargetFloor().value())) {
		this->ArrivedAtFloor();
	}
}

ICabin::direction IPhysicalCabin::GetDirection() {
	double abs = (double)GetCurrentTargetFloor().value() - position;
	if (abs < 0) {
		return ICabin::direction::Down;
	}
	else {
		return ICabin::direction::Up;
	}
}

double IPhysicalCabin::GetAcceleration() {
	double atLeastDistance = velosity * velosity / GetMaxAcceleration();
	if(GetDirection() == ICabin::direction::Up) {
		return GetMaxAcceleration() * (this->position + atLeastDistance >= GetCurrentTargetFloor() ? -1 : 1);
	}
	else {
		return GetMaxAcceleration() * (this->position - atLeastDistance <= GetCurrentTargetFloor() ? 1 : -1);
	}
}