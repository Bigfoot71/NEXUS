# Setting default options for LibSodium
set(SODIUM_DISABLE_TESTS ON CACHE BOOL "")

# Configuring the LibSodium library
add_subdirectory(${NEXUS_ROOT_PATH}/external/libsodium)

# Adding the LibSodium library to the links variable
list(APPEND NEXUS_EXTERNAL_LIBS sodium)

# Added the LibSodium inclusion directory to the inclusions variable
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/libsodium/src/libsodium/include)
