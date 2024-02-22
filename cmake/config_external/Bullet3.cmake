# Définition des options par défaut pour Bullet3
set(USE_GRAPHICAL_BENCHMARK OFF CACHE BOOL "")
set(INSTALL_CMAKE_FILES OFF CACHE BOOL "")
set(BUILD_BULLET2_DEMOS OFF CACHE BOOL "")
set(BUILD_UNIT_TESTS OFF CACHE BOOL "")
set(BUILD_CPU_DEMOS OFF CACHE BOOL "")
set(ENABLE_VHACD OFF CACHE BOOL "")
set(BUILD_EXTRAS OFF CACHE BOOL "")
set(INSTALL_LIBS OFF CACHE BOOL "")

# Configuration de la librairie Bullet3
add_subdirectory(${NEXUS_ROOT_PATH}/external/bullet3)

# Ajout de la libairie Bullet3 dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS BulletDynamics BulletCollision LinearMath)

# Ajout du repertoire d'inclusion de Bullet3 dans la variable des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/bullet3/src)
