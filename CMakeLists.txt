cmake_minimum_required(VERSION 3.20)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)

# Informujemy kompilator, gdzie leżą pliki nagłówkowe poszczególnych modułów
include_directories(math geometry lights core engine)

add_executable(Raytracing
        main.cpp
        math/Vector.cpp
        geometry/Sphere.cpp
        geometry/Plane.cpp
        geometry/Triangle.cpp
        lights/PointLight.cpp
        lights/SpotLight.cpp
        core/Image.cpp
        core/Ray.cpp
        core/Sampler.cpp
        engine/Camera.cpp
        engine/Scene.cpp
        engine/Renderer.cpp
)