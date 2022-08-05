# specter

specter is a raytracer that serves as a learning tool for graphics and engine development

## Development Status

The project is currently not usable by anyone but me. There is no GUI/CLI/manuals. The project
is still under feature development. Once these features are fletched out, I will implement a 
GUI. 
As of this writing I'm working on the GPU implementation of the LBVH data structure. The 
CPU reference implementation is working, however, that one uses recursion in the traversal
routine. The construction routine works without recursion. Once I've implemented the non-recursive traversal routine, I will work on the compute shader implementation. 
At that point, I will work on a custom scene file format, because working with OBJ is 
a nightmare. 

There are sitll some fundamental features missing, such as refraction and subsurface scattering.
Also, the sampling techniques are primitive to say the least. 

Finally, building this project is still not possible for anyone but myself. Once, I think the 
project is presentable I will provide a cmakefile.

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
