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

    - Scene descriptor does not adhere to some specification

## TODO

For patch 1.0.0

    - Fix memory leak in Octree destructor
    - Shrink size of Node structure
    - Clean the code
    - Add explanatory comments
    - Unify surface materials into one material type
    - Write complete parser for wavefront file format
    - Frame to frame convergence

For patch 1.0.1

    - Allow textures to be added to surfaces
    - Motion blur
    - Simple participating media
    - Refraction
    - Fluent, moving camera

For patch 1.0.2

    - Improved sampling techniques
    - 

For patch 2.0.0

    - This patch will focus on real-time performance. More later.

For patch 2.0.1 

    - This patch will introduce multiple camera models


Once the major issues are resolved work on the following features (in no particular order):

    - Support for Compute shader/CUDA acceleration
    - Frame to frame updating of image during ray tracing
    - Add GUI

## Fixed issues

    - [Fixed] The project has some bad coupling issues. These need to be refactored.
    - [Fixed] The ambient occlusion solver is not functional.
