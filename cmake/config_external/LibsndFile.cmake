# Setting default options for LibsndFile
set(BUILD_PROGRAMS OFF CACHE BOOL "Disable build libsndfile programs")
set(BUILD_EXAMPLES OFF CACHE BOOL "Disable build libsndfile examples")

# Configuring the LibsndFile library
add_subdirectory(${NEXUS_ROOT_PATH}/external/libsndfile)

# Adding the LibsndFile library to the links variable
list(APPEND NEXUS_EXTERNAL_LIBS sndfile)

# Added the LibsndFile inclusion directory to the inclusion variables
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/libsndfile/include)
