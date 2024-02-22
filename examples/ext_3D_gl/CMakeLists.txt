cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(gl_billboard_animation billboard_animation.cpp)
add_executable(gl_basic_light_3d basic_light_3d.cpp)
add_executable(gl_particles_3d particles_3d.cpp)
add_executable(gl_lights_3d lights_3d.cpp)
add_executable(gl_skybox skybox.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
