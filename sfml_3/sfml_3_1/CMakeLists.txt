cmake_minimum_required(VERSION 3.16)

add_executable(sfml_3_1 main.cpp)

target_link_libraries(sfml_3_1 PRIVATE SFML::Graphics SFML::Window SFML::System)