cmake_minimum_required(VERSION 3.22.1)

project(nexus)
set(CMAKE_CXX_STANDARD 17)

# Set the CMP0054 policy to avoid excessive variable expansion in conditionals.
# This is important to prevent unintended variable expansion.
cmake_policy(SET CMP0054 NEW)

# Retrieving the root directory of 'nexus'
set(NEXUS_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# Detects if 'nexus' is the main project
if("${CMAKE_SOURCE_DIR}" STREQUAL "${NEXUS_ROOT_PATH}")
    set(NEXUS_IS_MAIN TRUE)
endif()

# Defined the CMake modules directory
set(CMAKE_MODULE_PATH ${NEXUS_ROOT_PATH}/cmake)

# Predefining the variable for 'nexus' definitions
set(NEXUS_DEFINITIONS       "")

# Predefining source/inclusion/external link variables
# They will be updated by the cmake configuration modules
set(NEXUS_EXTERNAL_LIBS         "")
set(NEXUS_EXTERNAL_SOURCES      "")
set(NEXUS_EXTERNAL_INCLUDES     "")
set(NEXUS_EXTERNAL_DEFINITIONS  "")

# Recovery of configuration options
include(CMakeOptions.txt)

# Configuration according to the target platform
include(config_library/Platform)

# Configuring the desired graphics API
include(config_library/GraphicsAPI)

# Configuring 'nexus' definitions
include(config_library/Definitions)

# Configuring 'SDL2'
include(config_external/SDL2)

# Configuring 'OpenGL'
if(SUPPORT_OPENGL)
    include(config_external/OpenGL)
endif()

# Configuring 'OpenAL' and 'libsdnfile'
if(SUPPORT_AUDIO)
    include(config_external/OpenAL)
    include(config_external/LibsndFile)
endif()

# Configuring 'assimp'
if(SUPPORT_MODEL)
    include(config_external/Assimp)
endif()

# Configuring 'box2d'
if(SUPPORT_PHYSICS_2D)
    include(config_external/Box2D)
endif()

# Configuring 'bullet3'
if(SUPPORT_PHYSICS_3D)
    include(config_external/Bullet3)
endif()

# Configuring 'asio' and 'libsodium'
if(SUPPORT_NETWORK)
    include(config_external/ASIO)
    include(config_external/Libsodium)
endif()

# Recovery of 'nexus' sources
include(${NEXUS_ROOT_PATH}/source/CMakeLists.txt)

# Configure the main 'nexus' library
if(NEXUS_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED
        ${NEXUS_SOURCES} ${NEXUS_EXTERNAL_SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC
        ${NEXUS_SOURCES} ${NEXUS_EXTERNAL_SOURCES})
endif()

# Main and external inclusions for 'nexus'
target_include_directories(${PROJECT_NAME} PUBLIC
    ${NEXUS_ROOT_PATH}/include ${NEXUS_EXTERNAL_INCLUDES}
)

# Definitions for 'nexus'
target_compile_definitions(${PROJECT_NAME} PUBLIC
    ${NEXUS_DEFINITIONS} ${NEXUS_EXTERNAL_DEFINITIONS})

# Bindings of 'nexus' dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${NEXUS_EXTERNAL_LIBS}
)

# Configuring the samples
if (NEXUS_BUILD_EXAMPLES)
    add_subdirectory(${NEXUS_ROOT_PATH}/examples/basic)
    if(SUPPORT_AUDIO)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/audio)
    endif()
    if(EXTENSION_GFX)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/ext_gfx)
        if(EXTENSION_2D)
            add_subdirectory(${NEXUS_ROOT_PATH}/examples/ext_2D_gfx)
        endif()
    endif()
    if(SUPPORT_SOFTWARE_RASTERIZER)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/basic_sr)
    endif()
    if(SUPPORT_OPENGL)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/basic_gl)
        if(EXTENSION_2D)
            add_subdirectory(${NEXUS_ROOT_PATH}/examples/ext_2D_gl)
        endif()
        if(EXTENSION_3D AND SUPPORT_MODEL AND NOT GRAPHICS_API MATCHES "GL 1.1")
            add_subdirectory(${NEXUS_ROOT_PATH}/examples/ext_3D_gl)
        endif()
    endif()
    if(SUPPORT_PHYSICS_2D AND SUPPORT_OPENGL)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/physics_2D)
    endif()
    if(SUPPORT_PHYSICS_3D AND SUPPORT_OPENGL)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/physics_3D)
    endif()
    if(SUPPORT_NETWORK AND EXTENSION_GFX)
        add_subdirectory(${NEXUS_ROOT_PATH}/examples/network)
    endif()
endif()
