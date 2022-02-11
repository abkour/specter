/*
	This class implements methods to transform vertices from world to view space.
	It's purpose is to test the correctness of the raytracer and file loader through
	understandable primitive rasterization
*/
#pragma once
#include "vec3.hpp"
#include "mat4.hpp"

namespace specter {

enum class MovementDirection {
	Forward = 0, Backward, Left, Right, None
};

struct View {

	View(const vec3f& pos, const vec3f& dir);

	void look(float xoff, float yoff);
	void move(MovementDirection& direction, float timeElapsedSinceLastMove);

	void setMovementSpeed(const float newSpeed);

	float* getAddress();

	vec3f getPosition() const;
	vec3f getDirection() const;


protected:

	// We will use euler angles, because I don't yet understand quaternions.
	float pitch, yaw;
	float movementSpeed;

	vec3f pos, dir;
	mat4f view;
};

}