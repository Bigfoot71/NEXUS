set(PLATFORM_DEFINITIONS)

if(${PLATFORM} STREQUAL "Desktop")
    list(APPEND PLATFORM_DEFINITIONS -DPLATFORM_DESKTOP)
elseif(${PLATFORM} STREQUAL "Android")
    list(APPEND PLATFORM_DEFINITIONS -DPLATFORM_ANDROID)
endif()
