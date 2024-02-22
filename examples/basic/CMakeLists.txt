cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(surface_manipulation surface_manipulation.cpp)
add_executable(surface_generation surface_generation.cpp)
add_executable(draw_on_surface draw_on_surface.cpp)
add_executable(collision_2d collision_2d.cpp)
add_executable(window_icon window_icon.cpp)
add_executable(triangle triangle.cpp)
add_executable(app app.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
