# Définition des options par défaut pour LibsndFile
set(BUILD_PROGRAMS OFF CACHE BOOL "Disable build libsndfile programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable build libsndfile examples")

# Configuration de la librairie LibsndFile
add_subdirectory(${NEXUS_ROOT_PATH}/external/libsndfile)

# Ajout de la libairie LibsndFile dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS sndfile)

# Ajout du repertoire d'inclusion de LibsndFile dans la variables des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/libsndfile/include)
