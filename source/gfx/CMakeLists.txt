set(NEXUS_SOURCES_GRAPHICS
    source/gfx/nxSurface.cpp
)

if(EXTENSION_GFX)
    list(APPEND NEXUS_SOURCES_GRAPHICS
        source/gfx/ext_gfx/nxFont.cpp
        source/gfx/ext_gfx/nxTexture.cpp
        source/gfx/ext_gfx/nxRenderer.cpp
        source/gfx/ext_gfx/nxTargetTexture.cpp
        source/gfx/ext_gfx/nxVertexRenderer.cpp
        source/gfx/ext_gfx/nxStreamingTexture.cpp
    )
    if(EXTENSION_2D)
        list(APPEND NEXUS_SOURCES_GRAPHICS
            source/gfx/ext_gfx/ext_2D/nxSprite.cpp
        )
    endif()
endif()
