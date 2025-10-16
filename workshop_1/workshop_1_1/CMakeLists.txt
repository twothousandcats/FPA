cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

add_executable(workshop_1_1 main.cpp)

target_link_libraries(workshop_1_1 PRIVATE SFML::Graphics SFML::Window SFML::System)
