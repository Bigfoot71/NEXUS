# Définition des options par défaut pour SDL2
set(SDL_STATIC_ENABLED_BY_DEFAULT OFF CACHE BOOL "")
set(SDL_SHARED_ENABLED_BY_DEFAULT ON CACHE BOOL "")
set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "")

# Définition des options par défaut pour SDL2_image
set(SDL2IMAGE_INSTALL OFF CACHE BOOL "")
set(SDL2IMAGE_SAMPLES OFF CACHE BOOL "")

# Définition des options par défaut pour SDL2_ttf
set(SDL2TTF_INSTALL OFF CACHE BOOL "")
set(SDL2TTF_SAMPLES OFF CACHE BOOL "")

# Configuration des librairies SDL2/SDL2_image/SDL2_ttf
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2)
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2_ttf)
add_subdirectory(${NEXUS_ROOT_PATH}/external/sdl2_image)

# Ajout de liens dont depend SDL2 dans la variable de liens
if(WIN32 AND ${PLATFORM} STREQUAL "Desktop")
    list(APPEND NEXUS_EXTERNAL_LIBS
        SDL2main
    )
endif()

# Ajout des librairies dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS
    SDL2 SDL2_image SDL2_ttf freetype
)

# Ajout des repertoires d'inclusion dans la variables des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES
    ${NEXUS_ROOT_PATH}/external/sdl2/include
    ${NEXUS_ROOT_PATH}/external/sdl2_image
    ${NEXUS_ROOT_PATH}/external/sdl2_ttf
)
