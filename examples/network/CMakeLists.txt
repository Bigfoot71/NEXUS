cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
link_libraries(nexus)

add_executable(net_client client.cpp)
add_executable(net_server server.cpp)

add_compile_definitions(RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
