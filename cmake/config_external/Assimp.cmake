# Définition des options par défaut pour Assimp
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "")
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "")
set(ASSIMP_IGNORE_GIT_HASH ON CACHE BOOL "")
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "")
set(ASSIMP_INSTALL OFF CACHE BOOL "")

# TODO: Review the formats to include by default
set(ASSIMP_BUILD_COLLADA_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_M3D_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_FBX_IMPORTER ON CACHE BOOL "")
set(ASSIMP_BUILD_IQM_IMPORTER ON CACHE BOOL "")

# Disabled importer formats
set(ASSIMP_BUILD_AMF_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_BVH_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_OFF_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_COB_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_STL_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_3DS_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_AC_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASE_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_ASSBIN_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_B3D_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_DXF_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_CSM_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_HMP_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_IRRMESH_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_IRR_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_LWO_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_LWS_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MD2_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MD3_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MD5_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MDC_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MDL_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_NFF_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_NDO_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_OGRE_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_OPENGEX_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_PLY_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MS3D_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_BLEND_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_IFC_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_XGL_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_Q3D_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_Q3BSP_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_RAW_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_SIB_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_SMD_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_TERRAGEN_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_3D_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_X_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_X3D_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_3MF_IMPORTER OFF CACHE BOOL "")
set(ASSIMP_BUILD_MMD_IMPORTER OFF CACHE BOOL "")

# Configuration de la librairie Assimp
add_subdirectory(${NEXUS_ROOT_PATH}/external/assimp)

# Ajout de la libairie Assimp dans la variable de liens
list(APPEND NEXUS_EXTERNAL_LIBS assimp)

# Ajout du repertoire d'inclusion d'Assimp dans la variable des inclusions
list(APPEND NEXUS_EXTERNAL_INCLUDES ${NEXUS_ROOT_PATH}/external/assimp/include)