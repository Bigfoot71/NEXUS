# Setting default options for SDL2
set(SDL_STATIC_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
set(SDL_SHARED_ENABLED_BY_DEFAULT ON CACHE BOOL "")
set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "")

# Setting default options for SDL2_image
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "")
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "")

# Setting default options for SDL2_ttf
set(SDL2TTF_INSTALL OFF CACHE BOOL "")
set(SDL2TTF_SAMPLES OFF CACHE BOOL "")

# Configuring the SDL2/SDL2_image/SDL2_ttf libraries
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2)
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2_ttf)
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2_image)

# Adding links that SDL2 depends on in the links variable
if(WIN32 AND ${PLATFORM} STREQUAL "Desktop")
    list(APPEND NEXUS_EXTERNAL_LIBS
        SDL2main
    )
endif()

# Adding libraries to the links variable
list(APPEND NEXUS_EXTERNAL_LIBS
    SDL2 SDL2_image SDL2_ttf freetype
)

# Adding inclusion directories to the inclusion variables
list(APPEND NEXUS_EXTERNAL_INCLUDES
    ${NEXUS_ROOT_PATH}/external/sdl2/include
    ${NEXUS_ROOT_PATH}/external/sdl2_image
    ${NEXUS_ROOT_PATH}/external/sdl2_ttf
)
