cmake_minimum_required(VERSION 4.2)
project(Raytracing)

set(CMAKE_CXX_STANDARD 20)

add_executable(Raytracing main.cpp
        Vector.cpp
        Vector.h
        Ray.cpp
        Ray.h
        Sphere.cpp
        Sphere.h
        Plane.cpp
        Plane.h)
