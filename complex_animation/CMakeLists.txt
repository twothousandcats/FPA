cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(complex_animation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SFML_ROOT "/opt/homebrew/opt/sfml")

find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_subdirectory(01)
