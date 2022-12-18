#include "raster_engine.hpp"

#include "../window_manager.hpp"

#include <shaderdirect.hpp>

namespace specter {

using buffer_pair = std::pair<GLuint, GLuint>;

buffer_pair create_cube() {
    const float vertices[] =
    {
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    return { vao, vbo };
}

static DiffuseTextureMap create_diffuse_textures(
    const std::vector<filesystem::ObjMtlComponent>& input) 
{
    DiffuseTextureMap texture_map;
    std::set<std::size_t> unique_hashes;

    stbi_set_flip_vertically_on_load(true);
    for (const auto& mtlcomp : input) {
        for (int array_id = 0; const auto & hash : mtlcomp.tp_hashes) {
            auto [pos, accept] = unique_hashes.emplace(hash);
            auto tex_type = mtlcomp.texture_types[array_id];
            if (accept && tex_type == TextureType::Diffuse) {
                // Create the texture
                const auto& fn = mtlcomp.texture_paths[array_id];
                std::cout << "Opening file: " << fn.c_str() << "\n(Texture Type: Diffuse)\n";
                int x, y, nChannels;
                unsigned char* data = stbi_load(fn.c_str(), &x, &y, &nChannels, 0);
                if (data) {
                    GLenum format;
                    switch (nChannels) {
                    case 3:
                        format = GL_RGB;
                        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
                        break;
                    case 4:
                        format = GL_RGBA;
                        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
                        break;
                    default:
                        format = GL_NONE;
                        std::cerr << "Error format unaccepted, channel count: " << nChannels << "\n";
                        break;
                    }

                    // SPECTER_TODO: Fix texture loading/parsing error (see texture viewer for reference)
                    GLuint texture_id;
                    glGenTextures(1, &texture_id);
                    glBindTexture(GL_TEXTURE_2D, texture_id);
                    glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, format, GL_UNSIGNED_BYTE, data);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glGenerateMipmap(GL_TEXTURE_2D);

                    texture_map.emplace(hash, texture_id);

                    stbi_image_free(data);
                } else {
                    std::cerr << "Error loading file: " << fn.c_str() << '\n';
                }
            }
            array_id++;
        }
    }
    
    return texture_map;
}

static NormalTextureMap create_normal_textures(
    const std::vector<filesystem::ObjMtlComponent>& input)
{
    NormalTextureMap texture_map;
    std::set<std::size_t> unique_hashes;

    stbi_set_flip_vertically_on_load(true);
    for (const auto& mtlcomp : input) {
        for (int array_id = 0; const auto & hash : mtlcomp.tp_hashes) {
            auto [pos, emplace_success] = unique_hashes.emplace(hash);
            auto tex_type = mtlcomp.texture_types[array_id];
            if (emplace_success) {
                if (tex_type == TextureType::Bump) {
                    // Create the texture
                    const auto& fn = mtlcomp.texture_paths[array_id];
                    std::cout << "Opening file: " << fn.c_str() << "\n(Texture Type: Bump)\n";
                    int x, y, nChannels;
                    unsigned char* data = stbi_load(fn.c_str(), &x, &y, &nChannels, 1);
                    if (data) {
                        // SPECTER_TODO: Fix texture loading/parsing error (see texture viewer for reference)
                        GLuint texture_id;
                        glGenTextures(1, &texture_id);
                        glBindTexture(GL_TEXTURE_2D, texture_id);
                        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, x, y, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        glGenerateMipmap(GL_TEXTURE_2D);

                        texture_map.emplace(hash, texture_id);

                        stbi_image_free(data);
                    } else {
                        std::cerr << "Error loading file: " << fn.c_str() << '\n';
                    }
                }
                // Create the texture
            }
            array_id++;
        }
    }

    return texture_map;
}

static void delete_textures(DiffuseTextureMap& texture_map) {
    for (auto& [hash, id] : texture_map) {
        glDeleteTextures(1, &id);
    }
}

RasterRenderer::RasterRenderer(Scene& pScene) {
    scene = std::make_shared<Scene>(pScene);
}

void RasterRenderer::run() {
    
    WindowManager window_manager(  
            vec2u(scene->camera.resx(), scene->camera.resy()),
            WindowMode_::Windowed, true);

	glfwSetInputMode(window_manager.get_window_pointer(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    texture_viewer.init();

    glfwSwapInterval(1);

    auto window_handle = window_manager.get_window_pointer();

    auto indices = scene->model->GetFaces();
	auto nIndices = scene->model->GetFaceCount();
	
	std::vector<unsigned> vertexIndices;
	vertexIndices.resize(nIndices);
	for (int i = 0; i < vertexIndices.size(); ++i) {
		vertexIndices[i] = indices[i].p;
	}

    auto interleaved_data = scene->model->get_interleaved_data();

	GLuint vao, vbo, ebo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, interleaved_data.size() * sizeof(float), interleaved_data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(unsigned), vertexIndices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);   // aPos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), 0);
    glEnableVertexAttribArray(1);   // aNormal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);   // aTangent
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), reinterpret_cast<void*>(6 * sizeof(float)));
    glEnableVertexAttribArray(3);   // aBitangent
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), reinterpret_cast<void*>(9 * sizeof(float)));
    glEnableVertexAttribArray(4);   // aUV
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), reinterpret_cast<void*>(12 * sizeof(float)));

    // Textures
    material_components = scene->model->get_material_components();
    auto diffuse_maps = create_diffuse_textures(material_components);
    auto normal_maps = create_normal_textures(material_components);

    // Simple pass-through vertex shader that renders a texture onto a quad
    ShaderWrapper quadShader(
        false,
        shader_p(GL_VERTEX_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\hello_shader.glsl.vs")),
        shader_p(GL_FRAGMENT_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\hello_shader.glsl.fs"))
    );
	quadShader.bind();
    GLint textureIDs[] = { 0, 1 };
    quadShader.upload1iv(&textureIDs[0], "DiffuseTexture");
    quadShader.upload1iv(&textureIDs[1], "NormalTexture");

    // 
    // 
    // Cube
    auto [cube_vao, cube_vbo] = create_cube();

    ShaderWrapper cube_shader(
        false,
        shader_p(GL_VERTEX_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\simple_solid.glsl.vs")),
        shader_p(GL_FRAGMENT_SHADER, ROOT_DIRECTORY + std::string("\\src\\shaders\\simple_solid.glsl.fs"))
    );

    // Perspective transform
    mat4 perspective_transform = perspective(radians(45.f), (float)scene->camera.resx() / scene->camera.resy(), 0.1f, 100.f);

    bool V_Pressed = false;
    int texture_id_to_view = 0;

    float deltatime = 0.f;
    float lasttime = 0.f;
    while (!glfwWindowShouldClose(window_handle)) {
        glClearColor(0.05f, 0.05f, 0.05f, 0.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        deltatime = glfwGetTime() - lasttime;
        lasttime += deltatime;

        if (glfwGetKey(window_handle, GLFW_KEY_V) == GLFW_PRESS) {
            V_Pressed = true;
        }
        if (glfwGetKey(window_handle, GLFW_KEY_V) == GLFW_RELEASE && V_Pressed) {
            V_Pressed = false;
            texture_id_to_view++;
            //On_V_Pressed(texture_map, texture_id_to_view);
        }

        auto mouse_delta = window_manager.get_mouse_delta();
        if (mouse_delta.x != 0.f || mouse_delta.y != 0.f) {
            view.look(mouse_delta.x, mouse_delta.y);
            window_manager.reset_mouse_delta();
        }

        auto movementDirection = window_manager.get_movement_direction();
        if (movementDirection != MovementDirection::None) {
            view.move(movementDirection, deltatime * 2.f);
        }

        //On_V_Pressed(texture_map, texture_id_to_view);
        
        mat4f MVP = view.getUnderlying() * perspective_transform;
        vec3f LightPosition(0.f, 10.f, 0.f);
        vec3f ViewPosition = view.getPosition();

        // Scene
        quadShader.bind();
        quadShader.upload44fm(MVP.data, "MVP");
        quadShader.upload3fv(&LightPosition.x, "LightPosition");
        quadShader.upload3fv(&ViewPosition.x, "ViewPosition");
        glBindVertexArray(vao);
        std::size_t index_offset = 0;
        std::size_t index_count = 0;
        for (int i = 0; i < scene->model->GetNumberOfMeshes(); ++i) {
            auto material_name_hash = scene->model->getMaterialNameHash(i);
            int m = 0;
            for (auto it = material_components.begin(); it != material_components.end(); ++it) {
                if (material_name_hash == (*it).name_hash) {
                    break;
                }
                ++m;
            }
            for (int j = 0;j < material_components[m].texture_types.size(); ++j) {
                auto tt = material_components[m].texture_types[j];
                if (tt == TextureType::Diffuse) {
                    // All meshes are guaranteed to have diffuse textures, as long as the 
                    // model loading and asset pipeline are working correctly.
                    auto map_it = diffuse_maps.find(material_components[m].tp_hashes[j]);
                    auto [hash, tex_id] = *map_it;
                    glActiveTexture(GL_TEXTURE0);
                    glBindTexture(GL_TEXTURE_2D, tex_id);
                } else if (tt == TextureType::Bump) {
                    auto map_it = normal_maps.find(material_components[m].tp_hashes[j]);
                    // Handle meshes that don't have normal maps by breaking the loop execution.
                    if (map_it == normal_maps.end()) break;
                    auto [hash, tex_id] = *map_it;
                    glActiveTexture(GL_TEXTURE1);
                    glBindTexture(GL_TEXTURE_2D, tex_id);
                }
            }

            index_count = scene->model->GetFaceCountOfMesh(i);
            glDrawElements(GL_TRIANGLES, index_count, GL_UNSIGNED_INT, reinterpret_cast<void*>(index_offset * sizeof(unsigned)));
            index_offset += index_count;
        }

        // Cube
        mat4f model(1.f);
        model = scale(model, 0.2f);
        model = translate(model, LightPosition);
        mat4f cubeMVP = model * view.getUnderlying() * perspective_transform;
        cube_shader.bind();
        cube_shader.upload44fm(cubeMVP.data, "MVP");
        glBindVertexArray(cube_vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window_handle);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);

    delete_textures(diffuse_maps);
    delete_textures(normal_maps);
}

void RasterRenderer::On_V_Pressed(const DiffuseTextureMap& texture_map, int val) {
    if (val >= texture_map.size()) {
        val = val % texture_map.size();
    }
    int i = 0;
    for(auto it = texture_map.begin(); it != texture_map.end(); ++it) {
        if (i == val) {
            texture_viewer.display((*it).second);
            break;
        }
        ++i;
    }
}

}