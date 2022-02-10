#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"
#include "mat3.hpp"
#include "mat4.hpp"
#include "mesh.hpp"
#include "obj.hpp"
#include "camera.hpp"
#include "common_math.hpp"
#include "window.hpp"


int main(int argc, const char** argv) {
	try {
		static const char* filename = "C:\\Users\\flora\\rsc\\assets\\cube\\cube.obj";
		specter::ObjLoader objLoader(filename);
		const specter::vec3f eyepos(0.f, 0.f, -2.f);
		const specter::vec3f eyetarget(0.f);
		const specter::vec2f screen_resolution(728, 728);
		const float fov = specter::radians(45.f);
		specter::Camera camera(screen_resolution);
		camera.initializeVariables(eyepos, eyetarget, fov);
		
		std::vector<specter::vec3f> frame;
		frame.resize(specter::product(screen_resolution));

		for (int x = 0; x < screen_resolution.x; ++x) {
			for (int y = 0; y < screen_resolution.y; ++y) {
				const std::size_t index = y * screen_resolution.x + x;
				specter::Ray ray = camera.getRay(specter::vec2u(x + 1, y + 1));
				
				float mint = std::numeric_limits<float>::max();
				unsigned f = std::numeric_limits<unsigned>::max();

				for (int i = 0; i < objLoader.getTriangleCount(); ++i) {
					float u, v, t;
					if (objLoader.rayIntersection(ray, i, u, v, t)) {
						if (mint > t) {
							mint = t;
							f = i;
						}
					}
				}

				if (f != std::numeric_limits<unsigned>::max()) {
					specter::vec3f normal = objLoader.getNormal(f);
					frame[index] = normal;
				}
			}
		}

		specter::Window window(specter::WindowMode::BORDERLESS, specter::vec2u(1000, 500), "sometitle");
	}
	catch (std::runtime_error& e) {
		std::cout << e.what();
	}
	catch (...) {
		std::cout << "Unexpected error somewhere...";
	}
} 