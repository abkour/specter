#pragma once
#include <glad/glad.h>

#include <string>
#include <vector>

namespace specter {

struct Shader {

	Shader(std::initializer_list<std::pair<GLenum, std::string>>&& shaders)
		: m_shaders(shaders)
		, m_programId(0)
	{}

	~Shader();

	void bind() const;
	void create() const;

	GLuint id() const;

protected:

	std::vector<std::string> include_directories;
	std::vector<std::pair<GLenum, std::string>> m_shaders;

	GLuint m_programId;
};

}