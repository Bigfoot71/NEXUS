# Adding links that depend on ASIO in the links variable
if(UNIX)
    list(APPEND NEXUS_EXTERNAL_LIBS pthread)
elseif(WIN32 AND ${PLATFORM} STREQUAL "Desktop")
    list(APPEND NEXUS_EXTERNAL_LIBS ws2_32 wsock32)
endif()

# Added definitions for ASIO
list(APPEND ${NEXUS_EXTERNAL_DEFINITIONS}
    ASIO_STANDALONE ASIO_NO_DEPRECATED)

# Added ASIO inclusion directory to the inclusions variable
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/asio/asio/include)
