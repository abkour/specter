#pragma once
#include "texture.hpp"
#include "stb_image.h"

namespace specter {

class TextureImage : public ITexture {

public:

	TextureImage(const char* filename) {
		data = stbi_load(filename, &width, &height, &channels, 0);
		if (!data) {
			std::cerr << "Error loading file: " << filename << '\n';
		}
	}

	~TextureImage() {
		stbi_image_free(data);
	}

	virtual vec3f value(const float u, const float v, const vec3f& p) const override {
		// Subtracting 1e-6 is a hack. I will improve this later.
		int i = static_cast<int>(width * (u - 1e-6f)) % width;
		int j = static_cast<int>(height * (v - 1e-6f)) % height;
		if (i < 0) {
			i += width;
		}
		if (j < 0) {
			j += height;
		}
		const int idx = (i + j * width) * channels;
		const float alpha = channels == 4 ? (data[idx + 3] / 255.f) : 1.f;
		return alpha * (vec3f(data[idx], data[idx + 1], data[idx + 2]) / 255.f);
	}

	virtual unsigned char* get_data() const override {
		return data;
	}

protected:

	unsigned char* data;
	int width, height;
	int channels;
};

}