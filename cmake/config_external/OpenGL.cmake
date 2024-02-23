# Set OpenGL_GL_PREFERENCE to "GLVND" even when a legacy library exists,
# and if CMake version is less than or equal to 3.10.
# See https://cmake.org/cmake/help/latest/policy/CMP0072.html for more details.
if(POLICY CMP0072)
  cmake_policy(SET CMP0072 NEW)
endif()

# Set platform-specific configurations based on the selected NEXUS_PLATFORM.
if (${NEXUS_PLATFORM} MATCHES "Desktop")
    if (APPLE)
        # Force OpenGL 3.3 on macOS.
        find_library(OPENGL_LIBRARY OpenGL)
        set(LIBS_PRIVATE ${OPENGL_LIBRARY})
        link_libraries("${LIBS_PRIVATE}")
        if (NOT CMAKE_SYSTEM VERSION_LESS "18.0.0")
            add_definitions(-DGL_SILENCE_DEPRECATION)
            MESSAGE(AUTHOR_WARNING "OpenGL is deprecated starting with macOS 10.14 (Mojave)!")
        endif()
    elseif (WIN32)
        add_definitions(-D_CRT_SECURE_NO_WARNINGS)
        find_package(OpenGL QUIET)
        set(LIBS_PRIVATE ${OPENGL_LIBRARIES})
    elseif (UNIX)
        find_package(OpenGL QUIET)
        if ("${OPENGL_LIBRARIES}" STREQUAL "")
            set(OPENGL_LIBRARIES "GL")
        endif ()
        set(LIBS_PRIVATE m ${OPENGL_LIBRARIES})
    else ()
        find_package(OpenGL QUIET)
        if ("${OPENGL_LIBRARIES}" STREQUAL "")
            set(OPENGL_LIBRARIES "GL")
        endif ()
        set(LIBS_PRIVATE m ${OPENGL_LIBRARIES})

        if ("${CMAKE_SYSTEM_NAME}" MATCHES "(Net|Open)BSD")
            set(LIBS_PRIVATE ${OPENGL_LIBRARIES})
        endif ()

        if (NOT "${CMAKE_SYSTEM_NAME}" MATCHES "(Net|Open)BSD")
            set(LIBS_PRIVATE ${LIBS_PRIVATE})
        endif ()
    endif ()

elseif (${NEXUS_PLATFORM} MATCHES "Web")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s ASSERTIONS=1 --profiling")
    set(CMAKE_STATIC_LIBRARY_SUFFIX ".a")

elseif (${NEXUS_PLATFORM} MATCHES "Android")
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--build-id -Wl,-z,noexecstack -Wl,-z,relro -Wl,-z,now -Wl,--warn-shared-textrel")
    find_library(OPENGL_LIBRARY OpenGL)
    set(LIBS_PRIVATE m EGL GLESv2 OpenSLES)

elseif ("${NEXUS_PLATFORM}" MATCHES "DRM")
    add_definitions(-D_DEFAULT_SOURCE)
    add_definitions(-DEGL_NO_X11)
    add_definitions(-DPLATFORM_DRM)

    find_library(GLESV2 GLESv2)
    find_library(EGL EGL)
    find_library(DRM drm)
    find_library(GBM gbm)

    if (NOT CMAKE_CROSSCOMPILING)
        include_directories(/usr/include/libdrm)
    endif ()
    set(LIBS_PRIVATE ${GLESV2} ${EGL} ${DRM} ${GBM} m)
endif ()

# Addition of GLAD in sources and external inclusions of 'nexus' if necessary
if(NEXUS_PLATFORM STREQUAL "Desktop")
    if(NEXUS_GRAPHICS_API STREQUAL "GL ES2" OR NEXUS_GRAPHICS_API STREQUAL "GL ES3")
        list(APPEND NEXUS_EXTERNAL_SOURCES ${NEXUS_ROOT_PATH}/external/glad/src/gles2.c)
    else()
        list(APPEND NEXUS_EXTERNAL_SOURCES ${NEXUS_ROOT_PATH}/external/glad/src/gl.c)
    endif()
    list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/glad/include)
endif()
