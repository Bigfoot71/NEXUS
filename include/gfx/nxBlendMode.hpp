#ifndef NEXUS_GFX_BLEND_MODE_HPP
#define NEXUS_GFX_BLEND_MODE_HPP

#include "../platform/nxPlatform.hpp"

#include <SDL_blendmode.h>
#include <SDL_stdinc.h>

namespace nexus { namespace gfx {

    /**
     * @brief Enumeration for blending options used in rendering operations.
     *
     * This enumeration represents different blending options for combining source and destination colors during rendering.
     */
    enum class BlendMode : Uint32
    {
        None = SDL_BLENDMODE_NONE,      /**< No blending.
                                             dstRGBA = srcRGBA */

        Alpha = SDL_BLENDMODE_BLEND,    /**< Alpha blending.
                                             dstRGB = (srcRGB * srcA) + (dstRGB * (1-srcA))
                                             dstA = srcA + (dstA * (1-srcA)) */

        Additive = SDL_BLENDMODE_ADD,   /**< Additive blending.
                                             dstRGB = (srcRGB * srcA) + dstRGB
                                             dstA = dstA */

        Modulate = SDL_BLENDMODE_MOD,   /**< Color modulation.
                                             dstRGB = srcRGB * dstRGB
                                             dstA = dstA */

        Multiply = SDL_BLENDMODE_MUL,   /**< Color multiplication.
                                             dstRGB = (srcRGB * dstRGB) + (dstRGB * (1-srcA))
                                             dstA = dstA */

        Invalid = SDL_BLENDMODE_INVALID ///< Invalid blend mode.
    };

    /**
     * @brief Equality comparison operator for BlendMode and SDL_BlendMode.
     *
     * @param bm1 The first BlendMode operand.
     * @param bm2 The second SDL_BlendMode operand.
     * @return True if the blend modes are equal, false otherwise.
     */
    NEXUS_API constexpr bool operator==(BlendMode bm1, SDL_BlendMode bm2)
    {
        return static_cast<SDL_BlendMode>(bm1) == bm2;
    }

    /**
     * @brief Equality comparison operator for SDL_BlendMode and BlendMode.
     *
     * @param bm1 The first SDL_BlendMode operand.
     * @param bm2 The second BlendMode operand.
     * @return True if the blend modes are equal, false otherwise.
     */
    NEXUS_API constexpr bool operator==(SDL_BlendMode bm1, BlendMode bm2)
    {
        return bm2 == bm1;
    }

    /**
     * @brief Inequality comparison operator for BlendMode and SDL_BlendMode.
     *
     * @param bm1 The first BlendMode operand.
     * @param bm2 The second SDL_BlendMode operand.
     * @return True if the blend modes are not equal, false otherwise.
     */
    NEXUS_API constexpr bool operator!=(BlendMode bm1, SDL_BlendMode bm2)
    {
        return static_cast<SDL_BlendMode>(bm1) != bm2;
    }

    /**
     * @brief Inequality comparison operator for SDL_BlendMode and BlendMode.
     *
     * @param bm1 The first SDL_BlendMode operand.
     * @param bm2 The second BlendMode operand.
     * @return True if the blend modes are not equal, false otherwise.
     */
    NEXUS_API constexpr bool operator!=(SDL_BlendMode bm1, BlendMode bm2)
    {
        return bm2 != bm1;
    }

}}

#endif //NEXUS_GFX_BLEND_MODE_HPP
