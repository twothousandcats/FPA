add_executable(01 main.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Window Graphics System)

target_link_libraries(01 PRIVATE SFML::Graphics SFML::Window SFML::System)