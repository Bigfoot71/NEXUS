set(NEXUS_SOURCES_GRAPHICS_API)

if(NOT GRAPHICS_API STREQUAL "OFF")
    list(APPEND NEXUS_SOURCES_GRAPHICS
        source/gapi/cmn_impl/nxCamera2D.cpp
        source/gapi/cmn_impl/nxCamera3D.cpp
        source/gapi/cmn_impl/nxPrimitives2D.cpp
        source/gapi/cmn_impl/nxPrimitives3D.cpp
    )
    if(SUPPORT_MODEL)
        list(APPEND NEXUS_SOURCES_GRAPHICS
            source/gapi/cmn_impl/sp_model/nxModel.cpp
            source/gapi/cmn_impl/sp_model/nxMesh.cpp
        )
    endif()
endif()

if(SUPPORT_OPENGL)
    list(APPEND NEXUS_SOURCES_GRAPHICS_API
        source/gapi/gl/nxFont.cpp
        source/gapi/gl/nxUtils.cpp
        source/gapi/gl/nxContext.cpp
        source/gapi/gl/nxWindow.cpp
        source/gapi/gl/nxShader.cpp
        source/gapi/gl/nxTexture.cpp
        source/gapi/gl/nxCamera3D.cpp
        source/gapi/gl/nxExtensions.cpp
        source/gapi/gl/nxRenderBatch.cpp
        source/gapi/gl/nxTargetTexture.cpp
    )
    if(SUPPORT_MODEL)
        list(APPEND NEXUS_SOURCES_GRAPHICS_API
            source/gapi/gl/sp_model/nxModel.cpp
            source/gapi/gl/sp_model/nxMesh.cpp
        )
    endif()
    if(EXTENSION_2D)
        list(APPEND NEXUS_SOURCES_GRAPHICS_API
            source/gapi/gl/ext_2D/nxSprite2D.cpp
        )
    endif()
    if(EXTENSION_3D)
        list(APPEND NEXUS_SOURCES_GRAPHICS_API
            source/gapi/gl/ext_3D/nxSprite3D.cpp
        )
        if (NOT EXTENSIONS_2D)
            list(APPEND NEXUS_SOURCES_GRAPHICS_API
                source/gapi/gl/ext_2D/nxSprite2D.cpp    # bgSprite3D also depends on bgSprite2D
            )
        endif()
        if (NOT GRAPHICS_API STREQUAL "GL 1.1")
            list(APPEND NEXUS_SOURCES_GRAPHICS_API
                source/gapi/gl/ext_3D/nxLights3D.cpp
                source/gapi/gl/ext_3D/nxSkybox.cpp
            )
        endif()
    endif()
endif()

if(SUPPORT_SOFTWARE_RASTERIZER)
    list(APPEND NEXUS_SOURCES_GRAPHICS_API
        source/gapi/sr/nxTargetTexture.cpp
        source/gapi/sr/nxPipeline.cpp
        source/gapi/sr/nxCamera3D.cpp
        source/gapi/sr/nxContext.cpp
        source/gapi/sr/nxTexture.cpp
        source/gapi/sr/nxShader.cpp
        source/gapi/sr/nxWindow.cpp
        source/gapi/sr/nxFont.cpp
    )
    if(SUPPORT_MODEL)
        list(APPEND NEXUS_SOURCES_GRAPHICS_API
            source/gapi/sr/sp_model/nxModel.cpp
            source/gapi/sr/sp_model/nxMesh.cpp
        )
    endif()
endif()
