#include "view.hpp"
#include "common_math.hpp"

namespace specter {

View::View(const vec3f& pos, const vec3f& dir) {
	this->pos = pos;
	this->dir = dir;
	yaw = -89.f;
	pitch = 0.f;

	movementSpeed = 0.14f;

	view = lookAt(pos, dir, vec3f(0.f, 1.f, 0.f));
}

void View::look(float xoff, float yoff) {
	yaw += xoff * movementSpeed;
	pitch += yoff * movementSpeed;

	if (pitch > 89.f) {
		pitch = 89.f;
	}
	else if (pitch < -89.f) {
		pitch = -89.f;
	}

	dir.x = std::cos(radians(yaw)) * std::cos(radians(pitch));
	dir.y = std::sin(radians(pitch));
	dir.z = std::sin(radians(yaw)) * std::cos(radians(pitch));
	dir = normalize(dir);

	view = lookAt(pos, pos + dir, vec3f(0.f, 1.f, 0.f));
}

void View::move(MovementDirection& direction, float timeElapsedSinceLastMove) {
	switch (direction) {
	case MovementDirection::Forward:
		pos -= dir * timeElapsedSinceLastMove * movementSpeed;
		break;
	case MovementDirection::Backward:
		pos += dir * timeElapsedSinceLastMove * movementSpeed;
		break;
	case MovementDirection::Left:
		pos += cross(dir, vec3f(0.f, 1.f, 0.f)) * timeElapsedSinceLastMove * movementSpeed;
		break;
	case MovementDirection::Right:
		pos -= cross(dir, vec3f(0.f, 1.f, 0.f)) * timeElapsedSinceLastMove * movementSpeed;
		break;
	default:
		break;
	}

	view = lookAt(pos, pos + dir, vec3f(0.f, 1.f, 0.f));
}

void View::setMovementSpeed(const float newSpeed) {
	movementSpeed = newSpeed;
}


}