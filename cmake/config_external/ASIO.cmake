# Ajout de liens dont depend ASIO dans la variable de liens
if(UNIX)
    list(APPEND NEXUS_EXTERNAL_LIBS pthread)
elseif(WIN32 AND ${PLATFORM} STREQUAL "Desktop")
    list(APPEND NEXUS_EXTERNAL_LIBS ws2_32 wsock32)
endif()

# Ajout des definitions pour ASIO
list(APPEND ${NEXUS_EXTERNAL_DEFINITIONS}
    ASIO_STANDALONE ASIO_NO_DEPRECATED)

# Ajout du repertoire d'inclusion d'ASIO dans la variable des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/asio/asio/include)
