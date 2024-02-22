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
