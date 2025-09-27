add_executable(02 main_2.cpp)

find_package(SFML 3 REQUIRED COMPONENTS Window Graphics System)

target_link_libraries(02 PRIVATE SFML::Graphics SFML::Window SFML::System)