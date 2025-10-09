cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(sfml_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Указываем корень SFML
set(SFML_ROOT "/opt/homebrew/opt/sfml")

# Находим SFML
find_package(SFML 3 COMPONENTS Graphics Window System REQUIRED)

add_subdirectory(00)
add_subdirectory(01)
add_subdirectory(02)

add_subdirectory(sfml_3_1)