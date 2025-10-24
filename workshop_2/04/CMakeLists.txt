cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

add_executable(04 main.cpp)

target_link_libraries(04 PRIVATE SFML::Graphics SFML::Window SFML::System)
