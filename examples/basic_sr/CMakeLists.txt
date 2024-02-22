cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(sr_target_texture target_texture.cpp)
add_executable(sr_texture_shader texture_shader.cpp)
add_executable(sr_primitives_2d primitives_2d.cpp)
add_executable(sr_primitives_3d primitives_3d.cpp)
add_executable(sr_many_textures many_textures.cpp)
add_executable(sr_camera_2d camera_2d.cpp)
add_executable(sr_triangle triangle.cpp)
add_executable(sr_texture texture.cpp)
add_executable(sr_font_3d font_3d.cpp)

if(SUPPORT_MODEL)
    add_executable(sr_model_animation_3d model_animation_3d.cpp)
    add_executable(sr_model_3d model_3d.cpp)
endif()

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
