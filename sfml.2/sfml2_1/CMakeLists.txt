cmake_minimum_required(VERSION 3.16)
project(sfml2_1)

set(CMAKE_CXX_STANDARD 17)

# Пути к SFML
set(SFML_INCLUDE_DIR "/opt/homebrew/opt/sfml/include")
set(SFML_LIBRARY_DIR "/opt/homebrew/opt/sfml/lib")

# Добавляем заголовки
include_directories(${SFML_INCLUDE_DIR})

# Создаём исполняемый файл
add_executable(${PROJECT_NAME} main.cpp)

# Линкуем библиотеки напрямую
target_link_libraries(${PROJECT_NAME}
        ${SFML_LIBRARY_DIR}/libsfml-graphics.dylib
        ${SFML_LIBRARY_DIR}/libsfml-window.dylib
        ${SFML_LIBRARY_DIR}/libsfml-system.dylib
)