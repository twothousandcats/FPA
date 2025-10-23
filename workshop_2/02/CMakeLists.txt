cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

add_executable(02 main.cpp)

target_link_libraries(02 PRIVATE SFML::Graphics SFML::Window SFML::System)
