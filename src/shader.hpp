#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>

namespace specter {

// An abstraction of the OpenGL shader
struct Shader {

	// Constructs the shader pipeline through shadertype and shaderfile pairs.
	Shader(std::initializer_list<std::pair<GLenum, std::string>>&& shaders)
		: shaders(shaders)
		, programId(0)
	{}

	// Destroys the glProgram
	~Shader();

	// Create the shader. Errors in the shader code will throw an exception.
	void create();

	// Bind this shader as the currently active shader.
	void bind() const;
	// Retrieve the id of the underlying shader object.
	GLuint id() const;

protected:

	std::vector<std::pair<GLenum, std::string>> shaders;	// List of shader><file pairs
	GLuint programId; // The id of the underlying program object
};

}