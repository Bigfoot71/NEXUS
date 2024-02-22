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

# Defini le repertoire des modules CMake
set(CMAKE_MODULE_PATH ${NEXUS_ROOT_PATH}/cmake)

# Prédefinition de la variable des definitions de 'nexus'
set(NEXUS_DEFINITIONS       "")

# Prédefinition des variables de sources/inclusions/liens externes
# Elles seront mis à jour par les modules de configuration cmake
set(NEXUS_EXTERNAL_LIBS         "")
set(NEXUS_EXTERNAL_SOURCES      "")
set(NEXUS_EXTERNAL_INCLUDES     "")
set(NEXUS_EXTERNAL_DEFINITIONS  "")

# Recuperation des options de configuration
include(CMakeOptions.txt)

# Configuration selon la plateforme cible
include(config_library/Platform)

# Configuration de l'API graphique souhaitée
include(config_library/GraphicsAPI)

# Configuration des definitions de 'nexus'
include(config_library/Definitions)

# Configuration de 'SDL2'
include(config_external/SDL2)

# Configuration de 'OpenGL'
if(SUPPORT_OPENGL)
    include(config_external/OpenGL)
endif()

# Configuration de 'OpenAL' et de 'libsdnfile'
if(SUPPORT_AUDIO)
    include(config_external/OpenAL)
    include(config_external/LibsndFile)
endif()

# Configuration de 'assimp'
if(SUPPORT_MODEL)
    include(config_external/Assimp)
endif()

# Configuration de 'box2d'
if(SUPPORT_PHYSICS_2D)
    include(config_external/Box2D)
endif()

# Configuration de 'bullet3'
if(SUPPORT_PHYSICS_3D)
    include(config_external/Bullet3)
endif()

# Configuration de 'asio' et 'libsodium'
if(SUPPORT_NETWORK)
    include(config_external/ASIO)
    include(config_external/Libsodium)
endif()

# Recuperation des sources de 'nexus'
include(${NEXUS_ROOT_PATH}/source/CMakeLists.txt)

# Configure la librairie principal 'nexus'
if(NEXUS_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED
        ${NEXUS_SOURCES} ${NEXUS_EXTERNAL_SOURCES})
else()
    add_library(${PROJECT_NAME} STATIC
        ${NEXUS_SOURCES} ${NEXUS_EXTERNAL_SOURCES})
endif()

# Inclusions principale et external pour 'nexus'
target_include_directories(${PROJECT_NAME} PUBLIC
    ${NEXUS_ROOT_PATH}/include ${NEXUS_EXTERNAL_INCLUDES}
)

# Definitions pour 'nexus'
target_compile_definitions(${PROJECT_NAME} PUBLIC
    ${NEXUS_DEFINITIONS} ${NEXUS_EXTERNAL_DEFINITIONS})

# Liaisons des dépendances de 'nexus'
target_link_libraries(${PROJECT_NAME} PUBLIC
    ${NEXUS_EXTERNAL_LIBS}
)

# Configuration des exemples
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
