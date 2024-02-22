# Définition des options par défaut pour OpenAL
set(ALSOFT_TESTS OFF CACHE BOOL "Disable build and install test programs")
set(ALSOFT_UTILS OFF CACHE BOOL "Disable build and install utility programs")
set(ALSOFT_EXAMPLES OFF CACHE BOOL "Disable build and install example programs")
set(ALSOFT_INSTALL_UTILS OFF CACHE BOOL "Disable install utility programs (openal-info, alsoft-config, ...)")
set(ALSOFT_INSTALL_EXAMPLES OFF CACHE BOOL "Disable install example programs (alplay, alstream, ...)")
set(ALSOFT_INSTALL_CONFIG OFF CACHE BOOL "Disable install alsoft.conf sample configuration file")
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF CACHE BOOL "Disable install AmbDec preset files")
set(ALSOFT_INSTALL_HRTF_DATA OFF CACHE BOOL "Disable install HRTF definition files")
set(ALSOFT_INSTALL OFF CACHE BOOL "Disable install headers and libraries")

# Configuration de la librairie OpenAL
add_subdirectory(${NEXUS_ROOT_PATH}/external/openal-soft)

# Ajout de la libairie OpenAL dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS OpenAL)

# Ajout du repertoire d'inclusion d'OpenAL dans la variables des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/openal-soft/include)

# Ajout des defintions pour OpenAL
list(APPEND NEXUS_EXTERNAL_DEFINITIONS AL_ALEXT_PROTOTYPES)