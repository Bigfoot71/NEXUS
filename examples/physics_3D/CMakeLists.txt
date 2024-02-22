cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(phys3D_basic basic.cpp)

if(EXTENSION_3D)
    add_executable(phys3D_advanced_world advanced_world.cpp)
endif()

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
