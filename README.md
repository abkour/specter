# specter

specter is a raytracer that serves as a learning tool for graphics and engine development

## Usage

Project is broken and under development

## Dependenancies

    - GLFW is used for window setup
    - GLAD is used for loading OpenGL function pointers
    - nlohmann/json is used to parse scene descriptor files
    - oneAPI/tbb is used to add parallelism to the accelerating structure/s and ray tracing.

## Modules

Module descriptions go here

## Issues

    - The project has some bad coupling issues. These need to be refactored.
    - The ambient occlusion solver is not functional.
    - No GUI 
    - Test coverage is low and only limited to math code
    - Scene descriptor does not adhere to some specification

## TODO

Once the major issues are resolved work on the following features:

    - Support for Compute shader/CUDA acceleration
    - Frame to frame updating of image during ray tracing