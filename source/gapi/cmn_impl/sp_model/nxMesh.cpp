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

#include "gapi/cmn_impl/sp_model/nxMesh.hpp"

using namespace nexus;

_gapi_impl::VertexBoneData::VertexBoneData()
{
    std::fill(boneIDs.begin(), boneIDs.end(), -1);
    std::fill(weights.begin(), weights.end(), 0);
}

void _gapi_impl::VertexBoneData::AddBoneData(int boneID, float weight /*, bool& hadVertexError */)
{
    float minWeight = weight;
    Uint32 minWeightIx = 0;

    for (Uint32 i = 0; i < MaxBoneInfluence; ++i)
    {
        if (weights[i] < minWeight)
        {
            minWeightIx = i;
            minWeight = weights[i];
            if (minWeight == 0.0) break; // zero weight is the min - done
        }
    }

    if (minWeight > 0.0/* && !hadVertexError*/)
    {
        NEXUS_LOG(Warning) << "[gl::Mesh::VertexBoneData::AddBoneData] Too many weights/bones for a single vertex; using the " << MaxBoneInfluence << " largest weights\n";
        //**hadVertexError = true;
    }

    if (minWeight == weight) return; // no slot for this weight
    boneIDs[minWeightIx] = boneID;
    weights[minWeightIx] = weight;
}

void _gapi_impl::VertexBoneData::Normalize()
{
    float weightSum = 0.0f;
    for (Uint8 i = 0; i < MaxBoneInfluence; i++) weightSum += weights[i];
    for (Uint8 i = 0; i < MaxBoneInfluence; i++) weights[i] /= weightSum;
}
