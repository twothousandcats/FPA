add_executable(03 main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Window Graphics System)

target_link_libraries(03 PRIVATE SFML::Graphics SFML::Window SFML::System)