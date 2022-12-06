cmake_minimum_required(VERSION 3.13)

project(specter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

SET(SOURCE_LIST
    # core
    src/common.hpp
    src/imgui_wrapper.hpp
    src/misc.hpp
    src/movement_direction.hpp
    src/random_engine.hpp
    src/renderer.hpp
    src/rendering_types.hpp
    src/scene.hpp
    src/scene_descriptor.hpp
    src/shader.hpp
    src/timer.hpp
    src/view.hpp
    src/window.hpp
    src/window_manager.hpp
    # filesystem
    src/filesystem/convert_obj_to_sff.hpp
    src/filesystem/helpers.hpp
    src/filesystem/material_processor.hpp
    src/filesystem/model.hpp
    src/filesystem/sff.hpp
    # math
    src/math/common_math.hpp
    src/math/mat3.hpp
    src/math/mat4.hpp
    src/math/simd_math.hpp
    src/math/vec2.hpp
    src/math/vec3.hpp
    src/math/vec4.hpp
    # raster
    src/raster/raster_engine.hpp
    src/raster/texture_viewer.hpp
    # rtx
    src/rtx/aabb.hpp
    src/rtx/accel.hpp
    src/rtx/ambient_light.hpp
    src/rtx/area_light.hpp
    src/rtx/camera.hpp
    src/rtx/coordinate_system.hpp
    src/rtx/cpu_lbvh.hpp
    src/rtx/cpu_lbvh_helpers.hpp
    src/rtx/diffuse_light.hpp
    src/rtx/filters.hpp
    src/rtx/integrator.hpp
    src/rtx/integrator_normal.hpp
    src/rtx/integrator_pt.hpp
    src/rtx/intersection.hpp
    src/rtx/material_lambertian.hpp
    src/rtx/material_metal.hpp
    src/rtx/octree.hpp
    src/rtx/point_light.hpp
    src/rtx/ray.hpp
    src/rtx/sampler.hpp
    src/rtx/texture_single_color.hpp
    src/rtx/triangle.hpp

    #
    # CPP FILES
    #

    # core
    src/imgui_wrapper.cpp
    src/main.cpp
    src/misc.cpp
    src/renderer.cpp
    src/scene.cpp
    src/scene_descriptor.cpp
    src/shader.cpp
    src/timer.cpp
    src/view.cpp
    src/window.cpp
    src/window_manager.cpp
    # filesystem
    src/filesystem/model.cpp
    src/filesystem/model_loader.cpp
    # math
    src/math/common_math.cpp
    src/math/simd_math.cpp
    # raster
    src/raster/raster_engine.cpp
    src/raster/texture_viewer.cpp
    # rtx
    src/rtx/aabb.cpp
    src/rtx/accel.cpp
    src/rtx/ambient_light.cpp
    src/rtx/area_light.cpp
    src/rtx/camera.cpp
    src/rtx/coordinate_system.cpp
    src/rtx/cpu_lbvh.cpp
    src/rtx/cpu_lbvh_misc.cpp
    src/rtx/filters.cpp
    src/rtx/integrator_normal.cpp
    src/rtx/integrator_pt.cpp
    src/rtx/material_lambertian.cpp
    src/rtx/material_metal.cpp
    src/rtx/octree.cpp
    src/rtx/point_light.cpp
    src/rtx/ray.cpp
    src/rtx/sampler.cpp
    src/rtx/texture_single_color.cpp
    src/rtx/triangle.cpp
)

add_executable( specter
                ${SOURCE_LIST}) 

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_LIST})

add_subdirectory(ext/json)

include_directories("ext//glad//include//")
include_directories("ext//glfw-3.3.8//include//GLFW//")
include_directories("ext//glm//glm//")
include_directories("ext//imgui//")
include_directories("ext//json//include//")
include_directories("ext//oneTBB//include//")
include_directories("ext//shaderdirect//")
include_directories("ext//stb//")

# GLFW
target_link_libraries(specter debug "${CMAKE_SOURCE_DIR}/ext/glfw-3.3.8/build/src/Debug/glfw3.lib")
target_link_libraries(specter optimized "${CMAKE_SOURCE_DIR}/ext/glfw-3.3.8/build/src/Release/glfw3.lib")
# oneTBB
target_link_libraries(specter debug "${CMAKE_SOURCE_DIR}/ext/oneTBB/build//msvc_19.29_cxx_64_md_debug/tbb12_debug.lib")
target_link_libraries(specter optimized "${CMAKE_SOURCE_DIR}/ext/oneTBB/build/msvc_19.29_cxx_64_md_release/tbb12.lib")

target_sources(specter PUBLIC "ext/glad/src/glad.c")

# ImGUI requires we compile into our application
target_sources(specter PUBLIC "ext/imgui/imgui.cpp")
target_sources(specter PUBLIC "ext/imgui/imgui_demo.cpp")
target_sources(specter PUBLIC "ext/imgui/imgui_draw.cpp")
target_sources(specter PUBLIC "ext/imgui/imgui_tables.cpp")
target_sources(specter PUBLIC "ext/imgui/imgui_widgets.cpp")
target_sources(specter PUBLIC "ext/imgui/backends/imgui_impl_glfw.cpp")
target_sources(specter PUBLIC "ext/imgui/backends/imgui_impl_opengl3.cpp")
target_sources(specter PUBLIC "ext/imgui/backends/imgui_impl_win32.cpp")

add_compile_definitions(ROOT_DIRECTORY="${CMAKE_SOURCE_DIR}")