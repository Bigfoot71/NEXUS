cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(gfx_streaming_texture streaming_texture.cpp)
add_executable(gfx_vertex_renderer vertex_renderer.cpp)
add_executable(gfx_sprite_batch sprite_batch.cpp)
add_executable(gfx_renderer renderer.cpp)
add_executable(gfx_triangle triangle.cpp)
add_executable(gfx_texture texture.cpp)
add_executable(gfx_font font.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
