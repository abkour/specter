#pragma once
#include "texture_viewer.hpp"

namespace specter {

TextureViewer::TextureViewer()
	: vao(0)
	, vbo(0)
{}

TextureViewer::~TextureViewer() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

void TextureViewer::init() {
	float quad[] =
	{
		-0.75, -0.75,
		0.75, -0.75,
		0.75, 0.75,
		-0.75, -0.75,
		0.75, 0.75,
		-0.75, 0.75
	};

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), quad, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	Shader tmp_shader = {
		{ GL_VERTEX_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\texture_viewer.glsl.vs") },
		{ GL_FRAGMENT_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\texture_viewer.glsl.fs") }
	};

	shader = std::move(tmp_shader);
}

void TextureViewer::display(const int id) {
	shader.bind();
	glBindTexture(GL_TEXTURE_2D, id);
	glBindTextureUnit(0, id);
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TextureViewer::display(const int id,
	const float texture_x, const float texture_y,
	const float screen_x, const float screen_y) 
{
	const float clip_x = texture_x / screen_x;
	const float clip_y = texture_y / screen_y;
	
	float quad[] =
	{
		-clip_x, -clip_y,
		clip_x, -clip_y,
		clip_x, clip_y,
		-clip_x, -clip_y,
		clip_x, clip_y,
		-clip_x, clip_y
	};

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad), quad);
	
	glBindTexture(GL_TEXTURE_2D, id);
	glBindTextureUnit(0, id);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

}
