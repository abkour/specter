#include "filters.hpp"

namespace specter {

void grayscale_filter(unsigned char* output, const unsigned char* input, const vec2u imageResolution) {
	GLuint inputBuffer;
	glGenBuffers(1, &inputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, inputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, product(imageResolution) * sizeof(unsigned char) * 4, input, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, inputBuffer);

	GLuint outputBuffer;
	glGenBuffers(1, &outputBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, outputBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, product(imageResolution) * sizeof(unsigned char) * 4, NULL, GL_STATIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, outputBuffer);

	specter::Shader grayscaleShader =
	{
		{ GL_COMPUTE_SHADER, "C:\\Users\\flora\\source\\repos\\specter\\shaders\\grayscale_filter.glsl.comp" }
	};
	grayscaleShader.create();
	grayscaleShader.bind();

	glUniform1i(glGetUniformLocation(grayscaleShader.id(), "nPixels"), product(imageResolution));

	const int dispatchs = (product(imageResolution) / 1024.f) + 1;
	glDispatchCompute(dispatchs, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, product(imageResolution) * sizeof(unsigned char) * 4, output);
}

}
