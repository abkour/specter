#pragma once
#include <glad/glad.h>
#include "../shader.hpp"

#include <shaderdirect.hpp>

namespace specter {

class TextureViewer final {

public:

	TextureViewer();
	~TextureViewer();

	// Late initialization requirement, since we don't know when GLAD is loaded.
	void init();

	void display(const int id);

	// @texture_x: Number of pixels that the texture occupies in the x-direction
	// @texture_y: Number of pixels that the texture occupies in the y-direction
	// @screen_x: Number of pixels of the screen in the x-direction
	// @screen_y: Number of pixels of the screen in the y-direction
	void display(	const int id,
					const float texture_x, const float texture_y,
					const float screen_x, const float screen_y);

private:

	GLuint vao, vbo;
	Shader shader;
};

}