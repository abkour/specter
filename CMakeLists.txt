cmake_minimum_required(VERSION 3.13)

project(specter)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable( specter
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
                src/rtx/triangle.cpp) 

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