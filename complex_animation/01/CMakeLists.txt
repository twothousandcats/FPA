cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

add_executable(01 main.cpp)

target_link_libraries(01 PRIVATE SFML::Graphics SFML::Window SFML::System)
