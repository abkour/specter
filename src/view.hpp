/*
	This class implements methods to transform vertices from world to view space.
	It's purpose is to test the correctness of the raytracer and file loader through
	understandable primitive rasterization
*/
#pragma once
#include "math/vec3.hpp"
#include "math/mat4.hpp"

namespace specter {

// Supported directions of movement for the camera 
enum class MovementDirection {
	Forward = 0, Backward, Left, Right, None
};

struct View {

	// Initialize the world-to-view matrix through camera position and direction.
	View(const vec3f& pos, const vec3f& dir);

	// Convert cursor offsets (xoff, yoff) into changes of the direction vector of the camera.
	// At the moment eular angles are used. However, in the future support for quaternions might be added.
	void look(float xoff, float yoff);
	// Move the camera position in the direction specified by \direction.
	// Corrects differences in frame time to allow for smooth movement 
	void move(MovementDirection& direction, float timeElapsedSinceLastMove);

	// Manually set the movement spped of the camera
	void setMovementSpeed(const float newSpeed);

	// Get the address of the underlying world-to-view matrix
	float* getAddress();
	
	// Returns the position of the camera
	vec3f getPosition() const;
	// Returns the direction of the camera
	vec3f getDirection() const;

protected:

	// We will use euler angles to rotate the camera, because I don't yet understand quaternions.
	float pitch, yaw;
	// Movement speed of the camera
	float movementSpeed;

	// Position and normalized direction of the camera
	vec3f pos, dir;
	// world-to-view matrix
	mat4f view;
};

}