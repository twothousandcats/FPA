cmake_minimum_required(VERSION 3.16)

add_executable(00 main.cpp)

# Линкуем через правильные цели SFML 3.0: SFML::Graphics и т.д.
target_link_libraries(00 PRIVATE SFML::Graphics SFML::Window SFML::System)