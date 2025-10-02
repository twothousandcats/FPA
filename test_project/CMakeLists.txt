cmake_minimum_required(VERSION 3.15)
project(test_project)

set(CMAKE_CXX_STANDARD 17)

set(SFML_ROOT /opt/homebrew)

find_path(SFML_INCLUDE_DIR SFML/Graphics.hpp
        PATHS ${SFML_ROOT}/include
        NO_DEFAULT_PATH
)

find_library(SFML_GRAPHICS_LIB sfml-graphics PATHS ${SFML_ROOT}/lib NO_DEFAULT_PATH)
find_library(SFML_WINDOW_LIB sfml-window   PATHS ${SFML_ROOT}/lib NO_DEFAULT_PATH)
find_library(SFML_SYSTEM_LIB sfml-system   PATHS ${SFML_ROOT}/lib NO_DEFAULT_PATH)

if(NOT SFML_INCLUDE_DIR OR NOT SFML_GRAPHICS_LIB)
    message(FATAL_ERROR "SFML not found! Check SFML_ROOT path.")
endif()

include_directories(${SFML_INCLUDE_DIR})

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME}
        ${SFML_GRAPHICS_LIB}
        ${SFML_WINDOW_LIB}
        ${SFML_SYSTEM_LIB}
)