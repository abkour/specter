#pragma once
#include "../scene.hpp"
#include "../shader.hpp"
#include "../view.hpp"
#include "../window.hpp"

#include "texture_viewer.hpp"

namespace specter {

using DiffuseTextureMap = std::map<std::size_t, GLuint>;
using NormalTextureMap = std::map<std::size_t, GLuint>;

class RasterRenderer final {

public:

    RasterRenderer(Scene& pScene);

    void run();

private:

    TextureViewer texture_viewer;

    std::shared_ptr<Scene> scene;
    //Window window_manager;
    View view;

private:

    filesystem::ObjMtlComponents material_components;
    // Implementation details
    void On_V_Pressed(const DiffuseTextureMap& texture_map, int val);
};

}