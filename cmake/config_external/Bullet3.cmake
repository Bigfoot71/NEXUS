# Setting default options for Bullet3
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "")
set(INSTALL_CMAKE_FILES OFF CACHE BOOL "")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "")
set(ENABLE_VHACD OFF CACHE BOOL "")
set(BUILD_EXTRAS OFF CACHE BOOL "")
set(INSTALL_LIBS OFF CACHE BOOL "")

# Configuring the Bullet3 library
add_subdirectory(${NEXUS_ROOT_PATH}/external/bullet3)

# Adding the Bullet3 library to the links variable
list(APPEND NEXUS_EXTERNAL_LIBS BulletDynamics BulletCollision LinearMath)

# Added Bullet3 include directory to the includes variable
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/bullet3/src)
