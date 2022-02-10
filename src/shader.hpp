#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>

namespace specter {

struct Shader {

	Shader(std::initializer_list<std::pair<GLenum, std::string>>&& shaders)
		: shaders(shaders)
		, programId(0)
	{}

	~Shader();

	void create();

	void bind() const;
	GLuint id() const;

protected:

	std::vector<std::pair<GLenum, std::string>> shaders;
	GLuint programId;
};

}