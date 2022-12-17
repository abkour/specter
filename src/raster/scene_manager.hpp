#pragma once
#include "../filesystem/model.hpp"
#include "../scene.hpp"
#include "raster_engine.hpp"

namespace specter {

class SceneManager final {

public:

	SceneManager() = delete;

	SceneManager(Scene& scene);



private:

	RasterRenderer rendering_engine;
};

}