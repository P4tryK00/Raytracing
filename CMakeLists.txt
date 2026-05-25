cmake_minimum_required(VERSION 4.2)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)


add_executable(Raytracing
        main.cpp
        src/Camera.cpp
        src/Image.cpp
        src/Plane.cpp
        src/Ray.cpp
        src/Sphere.cpp
        src/Triangle.cpp
        src/Vector.cpp
        src/Scene.cpp
        src/Sampler.cpp
        src/Renderer.cpp

)

target_include_directories(Raytracing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers)
