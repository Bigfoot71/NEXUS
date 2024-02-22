# Définition de la variable en fonction de GRAPHICS_API
set(GRAPHICS_API_DEFINITIONS)

if(${GRAPHICS_API} STREQUAL "OFF")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_NONE)

elseif(${GRAPHICS_API} STREQUAL "ALL")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_SOFT)
    if(${PLATFORM} STREQUAL "Desktop")
        list(APPEND GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_33)
    else()
        list(APPEND GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_ES2)
    endif()

elseif(${GRAPHICS_API} STREQUAL "SOFT")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_SOFT)

elseif(${GRAPHICS_API} STREQUAL "GL 1.1")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_11)

elseif(${GRAPHICS_API} STREQUAL "GL 2.1")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_21)

elseif(${GRAPHICS_API} STREQUAL "GL 3.3")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_33)

elseif(${GRAPHICS_API} STREQUAL "GL 4.3")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_43)

elseif(${GRAPHICS_API} STREQUAL "GL ES2")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_ES2)

elseif(${GRAPHICS_API} STREQUAL "GL ES3")
    set(GRAPHICS_API_DEFINITIONS -DGRAPHICS_API_OPENGL_ES3)

endif()

# Pré-définition des indicateur d'utilisation d'API graphique
set(SUPPORT_SOFTWARE_RASTERIZER OFF)
set(SUPPORT_OPENGL OFF)

# Detection du type d'API graphique utilisé
if (${GRAPHICS_API} STREQUAL "ALL")
    set(SUPPORT_SOFTWARE_RASTERIZER ON)
    set(SUPPORT_OPENGL ON)

elseif(${GRAPHICS_API} STREQUAL "SOFT")
    set(SUPPORT_SOFTWARE_RASTERIZER ON)

elseif(GRAPHICS_API MATCHES ".*GL.*")
    set(SUPPORT_OPENGL ON)

endif()

# Application du type d'API graphique en tant que definition
if (SUPPORT_SOFTWARE_RASTERIZER)
    add_compile_definitions(SUPPORT_SOFTWARE_RASTERIZER)
endif()
if(SUPPORT_OPENGL)
    add_compile_definitions(SUPPORT_OPENGL)
endif()


