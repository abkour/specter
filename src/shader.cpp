#pragma once
#include "shader.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace specter {

static void verifyShaderIntegrity(GLenum shaderType, GLuint shaderId, GLenum status);
static void verifyProgramIdentity(GLuint programId);

Shader::~Shader() {
	glDeleteProgram(programId);
}

void Shader::bind() const {
	glUseProgram(programId);
}

void Shader::create() {
	std::vector<GLuint> shaderIds;
	shaderIds.reserve(shaders.size());

	for (const auto& shader : shaders) {
		shaderIds.push_back(glCreateShader(std::get<GLenum>(shader)));
		
		std::ifstream shaderFile(std::get<std::string>(shader));
		if (shaderFile.fail()) {
			throw std::runtime_error("Filename " + std::get<std::string>(shader) + " does not exist!");
		}

		std::stringstream shaderSource;
		shaderSource << shaderFile.rdbuf();
		shaderFile.close();

		const auto shaderSourceString = shaderSource.str();
		const auto shaderSourceCString = shaderSourceString.c_str();

		glShaderSource(shaderIds.back(), 1, &shaderSourceCString, NULL);
		glCompileShader(shaderIds.back());

		try {
			verifyShaderIntegrity(std::get<GLenum>(shader), shaderIds.back(), GL_COMPILE_STATUS);
		} catch (std::runtime_error& e) {
			std::cout << e.what();
			std::for_each(shaderIds.begin(), shaderIds.end(), [](GLuint shaderId) { glDeleteShader(shaderId); });
			throw std::runtime_error("Shader creation failed!");
		} catch (...) {
			std::for_each(shaderIds.begin(), shaderIds.end(), [](GLuint shaderId) { glDeleteShader(shaderId); });
			throw std::runtime_error("Unexpected error during shader validation stage. Shader creation failed!");
		}
	}

	programId = glCreateProgram();
	std::for_each(shaderIds.begin(), shaderIds.end(), [&](GLuint shaderId) { glAttachShader(programId, shaderId); });
	glLinkProgram(programId);
	std::for_each(shaderIds.begin(), shaderIds.end(), [&](GLuint shaderId) { glDetachShader(programId, shaderId); });

	try {
		verifyProgramIdentity(programId);
	} catch (std::runtime_error& e) {
		std::cout << e.what();
		std::for_each(shaderIds.begin(), shaderIds.end(), [](GLuint shaderId) { glDeleteShader(shaderId); });
		throw std::runtime_error("Shader creation failed!");
	} catch (...) {
		std::for_each(shaderIds.begin(), shaderIds.end(), [](GLuint shaderId) { glDeleteShader(shaderId); });
		throw std::runtime_error("Unexpected error during shader validation stage. Shader creation failed!");
	}

	std::for_each(shaderIds.begin(), shaderIds.end(), [](GLuint shaderId) { glDeleteShader(shaderId); });
}

GLuint Shader::id() const {
	return programId;
}

static void verifyShaderIntegrity(GLenum shaderType, GLuint shaderId, GLenum status) {
	int success;
	glGetShaderiv(shaderId, status, &success);
	if (success != GL_TRUE) {
		char errorLog[512];
		glGetShaderInfoLog(shaderId, 512, NULL, errorLog);
		std::string shaderStage;
		switch (shaderType) {
		case GL_VERTEX_SHADER:
			shaderStage = "Vertex shader";
			break;
		case GL_TESS_CONTROL_SHADER:
			shaderStage = "Tessellation control shader";
			break;
		case GL_TESS_EVALUATION_SHADER:
			shaderStage = "Tessellation evaluation shader";
			break;
		case GL_GEOMETRY_SHADER:
			shaderStage = "Geometry shader";
			break;
		case GL_FRAGMENT_SHADER:
			shaderStage = "Fragment shader";
			break;
		case GL_COMPUTE_SHADER:
			shaderStage = "Compute shader";
			break;
		default:
			shaderStage = "BadEnum";
			break;
		}

		throw std::runtime_error(shaderStage + ", error: " + errorLog);
	}
}

static void verifyProgramIdentity(GLuint programId) {
	int success;
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (success != GL_TRUE) {
		char errorLog[512];
		glGetShaderInfoLog(programId, 512, NULL, errorLog);
		throw std::runtime_error(std::string("Shader program failed linking, error: ") + errorLog);
	}
}

}