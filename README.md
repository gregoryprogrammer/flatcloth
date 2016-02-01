# Flatcloth

Simple 3D engine to experiment with flat meshes that have constraints between
vertices - clothes.

There is also an attempt to ECS (entity component system) with variadic templates.

# Dependencies

* C++14
* SDL2
* OpenGL

# Controls

* WSAD + mouse - first person movement
* right mouse button - set some force on clothes
* left mouse button - grab cloth and tear - **A BIG TODO**
* C - toggle fly mode
* P - toggle pause physics
* F - toggle fullscreen

# Build and run, on Linux

    git clone https://github.com/gregoryprogrammer/flatcloth.git
    mkdir flatcloth/build
    cd flatcloth/build
    cmake ..
    make
    ./flatcloth

# TODO

* cloth tearing
* nice GLSL with lighting
* constraints satisfying optimization

