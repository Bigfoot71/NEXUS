/**
 * Copyright (c) 2023-2024 Le Juez Victor
 *
 * This software is provided "as-is", without any express or implied warranty. In no event 
 * will the authors be held liable for any damages arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose, including commercial 
 * applications, and to alter it and redistribute it freely, subject to the following restrictions:
 *
 *   1. The origin of this software must not be misrepresented; you must not claim that you 
 *   wrote the original software. If you use this software in a product, an acknowledgment 
 *   in the product documentation would be appreciated but is not required.
 *
 *   2. Altered source versions must be plainly marked as such, and must not be misrepresented
 *   as being the original software.
 *
 *   3. This notice may not be removed or altered from any source distribution.
 */

#ifndef NEXUS_HPP
#define NEXUS_HPP

// core
#include "core/nxApp.hpp"
#include "core/nxLog.hpp"
#include "core/nxText.hpp"
#include "core/nxState.hpp"
#include "core/nxClock.hpp"
#include "core/nxEvent.hpp"
#include "core/nxRandom.hpp"
#include "core/nxWindow.hpp"
#include "core/nxException.hpp"
#include "core/nxFileFormat.hpp"
#include "core/nxFileSystem.hpp"
#ifdef EXTENSION_CORE
#   include "core/ext_core/nxAssetManager.hpp"
#   include "core/ext_core/nxSaveManager.hpp"
#endif

// gfx
#include "gfx/nxPixel.hpp"
#include "gfx/nxColor.hpp"
#include "gfx/nxSurface.hpp"
#include "gfx/nxBasicFont.hpp"
#ifdef EXTENSION_GFX
#   include "gfx/ext_gfx/nxApp.hpp"
#   include "gfx/ext_gfx/nxFont.hpp"
#   include "gfx/ext_gfx/nxTexture.hpp"
#   include "gfx/ext_gfx/nxRenderer.hpp"
#   include "gfx/ext_gfx/nxTargetTexture.hpp"
#   include "gfx/ext_gfx/nxVertexRenderer.hpp"
#   include "gfx/ext_gfx/nxStreamingTexture.hpp"
#   ifdef EXTENSION_2D
#       include "gfx/ext_gfx/ext_2D/nxParticles.hpp"
#       include "gfx/ext_gfx/ext_2D/nxSprite.hpp"
#   endif
#endif

// graphics-api
#ifdef SUPPORT_OPENGL
#   include "gapi/gl/nxApp.hpp"
#   include "gapi/gl/nxFont.hpp"
#   include "gapi/gl/nxUtils.hpp"
#   include "gapi/gl/nxEnums.hpp"
#   include "gapi/gl/nxConfig.hpp"
#   include "gapi/gl/nxShader.hpp"
#   include "gapi/gl/nxWindow.hpp"
#   include "gapi/gl/nxContext.hpp"
#   include "gapi/gl/nxTexture.hpp"
#   include "gapi/gl/nxCamera2D.hpp"
#   include "gapi/gl/nxCamera3D.hpp"
#   include "gapi/gl/nxContextual.hpp"
#   include "gapi/gl/nxExtensions.hpp"
#   include "gapi/gl/nxRenderBatch.hpp"
#   include "gapi/gl/nxPrimitives2D.hpp"
#   include "gapi/gl/nxPrimitives3D.hpp"
#   include "gapi/gl/nxTargetTexture.hpp"
#   ifdef SUPPORT_MODEL
#       include "gapi/gl/sp_model/nxMaterial.hpp"
#       include "gapi/gl/sp_model/nxModel.hpp"
#       include "gapi/gl/sp_model/nxMesh.hpp"
#   endif
#   ifdef EXTENSION_2D
#       include "gapi/gl/ext_2D/nxParticles2D.hpp"
#       include "gapi/gl/ext_2D/nxSprite2D.hpp"
#   endif
#   if defined(EXTENSION_3D)
#       include "gapi/gl/ext_3D/nxParticles3D.hpp"
#       include "gapi/gl/ext_3D/nxSprite3D.hpp"
#       if defined(SUPPORT_MODEL) && !defined(GRAPHICS_API_OPENGL_11)
#           include "gapi/gl/ext_3D/nxLights3D.hpp"
#           include "gapi/gl/ext_3D/nxSkybox.hpp"
#       endif
#   endif
#endif
#ifdef SUPPORT_SOFTWARE_RASTERIZER
#   include "gapi/sr/nxApp.hpp"
#   include "gapi/sr/nxFont.hpp"
#   include "gapi/sr/nxEnums.hpp"
#   include "gapi/sr/nxShader.hpp"
#   include "gapi/sr/nxWindow.hpp"
#   include "gapi/sr/nxContext.hpp"
#   include "gapi/sr/nxTexture.hpp"
#   include "gapi/sr/nxCamera2D.hpp"
#   include "gapi/sr/nxCamera3D.hpp"
#   include "gapi/sr/nxPipeline.hpp"
#   include "gapi/sr/nxContextual.hpp"
#   include "gapi/sr/nxPrimitives2D.hpp"
#   include "gapi/sr/nxPrimitives3D.hpp"
#   include "gapi/sr/nxTargetTexture.hpp"
#   ifdef SUPPORT_MODEL
#       include "gapi/sr/sp_model/nxMaterial.hpp"
#       include "gapi/sr/sp_model/nxModel.hpp"
#       include "gapi/sr/sp_model/nxMesh.hpp"
#   endif
#endif

// audio
#ifdef SUPPORT_AUDIO
#   include "audio/nxDevice.hpp"
#   include "audio/nxEffect.hpp"
#   include "audio/nxSource.hpp"
#   include "audio/nxMusic.hpp"
#   include "audio/nxSound.hpp"
#endif

// inputs
#include "input/nxKeyboard.hpp"
#include "input/nxGamepad.hpp"
#include "input/nxMouse.hpp"
#include "input/nxTouch.hpp"

// math
#include "math/nxMath.hpp"
#include "math/nxEase.hpp"
#include "math/nxMat2.hpp"
#include "math/nxMat3.hpp"
#include "math/nxMat4.hpp"
#include "math/nxVec2.hpp"
#include "math/nxVec3.hpp"
#include "math/nxVec4.hpp"

// shapes 2D
#include "shape/2D/nxAABB.hpp"
#include "shape/2D/nxLine.hpp"
#include "shape/2D/nxMesh.hpp"
#include "shape/2D/nxCircle.hpp"
#include "shape/2D/nxEllipse.hpp"
#include "shape/2D/nxPolygon.hpp"
#include "shape/2D/nxTriangle.hpp"
#include "shape/2D/nxRectangle.hpp"
#include "shape/2D/nxFunctions.hpp"

// shapes 3D
#include "shape/3D/nxRay.hpp"
#include "shape/3D/nxLine.hpp"
#include "shape/3D/nxMesh.hpp"
#include "shape/3D/nxAABB.hpp"
#include "shape/3D/nxCube.hpp"
#include "shape/3D/nxPlane.hpp"
#include "shape/3D/nxSphere.hpp"
#include "shape/3D/nxCapsule.hpp"
#include "shape/3D/nxCylinder.hpp"

// physics 2D
#ifdef SUPPORT_PHYSICS_2D
#   include "phys/2D/nxPhysics.hpp"
#   include "phys/2D/nxDrawing.hpp"
#   include "phys/2D/nxShapes.hpp"
#endif

// physics 3D
#ifdef SUPPORT_PHYSICS_3D
#   include "phys/3D/rigid_objects/nxRigidCapsule.hpp"
#   include "phys/3D/rigid_objects/nxRigidSphere.hpp"
#   include "phys/3D/rigid_objects/nxRigidModel.hpp"
#   include "phys/3D/rigid_objects/nxRigidCube.hpp"
#   include "phys/3D/rigid_objects/nxRigidMesh.hpp"
#   include "phys/3D/nxWorld.hpp"
#endif

// network
#ifdef SUPPORT_NETWORK
#   include "net/nxClientInterface.hpp"
#   include "net/nxServerInterface.hpp"
#   include "net/nxConnection.hpp"
#   include "net/nxSecurity.hpp"
#   include "net/nxPacket.hpp"
#endif

// utils
#include "utils/nxContextual.hpp"
#include "utils/nxThreadSafeQueue.hpp"

#endif //NEXUS_HPP