# Définition des options par défaut pour LibSodium
set(SODIUM_DISABLE_TESTS ON CACHE BOOL "")

# Configuration de la librairie LibSodium
add_subdirectory(${NEXUS_ROOT_PATH}/external/libsodium)

# Ajout de la libairie LibSodium dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS sodium)

# Ajout du repertoire d'inclusion de LibSodium dans la variable des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/libsodium/src/libsodium/include)
