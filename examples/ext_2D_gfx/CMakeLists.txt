cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(gfx_particles_texture particles_texture.cpp)
add_executable(gfx_sprite_instances sprite_instances.cpp)
add_executable(gfx_particles particles.cpp)
add_executable(gfx_sprite sprite.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
