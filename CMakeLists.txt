cmake_minimum_required(VERSION 3.29)
project(projekt_snake)

set(CMAKE_CXX_STANDARD 20)

add_executable(projekt_snake
        display.cpp
        display.hpp
        snake_body.cpp
        snake_body.hpp
        Fruits.cpp
        Fruits.hpp
        main.cpp)
