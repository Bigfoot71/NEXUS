cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(music music.cpp)
add_executable(sound sound.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
