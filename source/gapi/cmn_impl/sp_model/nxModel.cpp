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

#include "gapi/cmn_impl/sp_model/nxModel.hpp"
#include "math/nxMat4.hpp"

using namespace nexus;

/* IMPL BONE */

_gapi_impl::Bone::Bone(int boneID, const aiNodeAnim* channel)
: localTransform(math::Mat4::Identity())
, id(boneID)
{
    positions.reserve(channel->mNumPositionKeys);
    for (int i = 0; i < channel->mNumPositionKeys; i++)
    {
        const aiVectorKey &keyFrame = channel->mPositionKeys[i];
        positions.emplace_back(keyFrame.mValue, keyFrame.mTime);
    }

    rotations.reserve(channel->mNumRotationKeys);
    for (int i = 0; i < channel->mNumRotationKeys; i++)
    {
        const aiQuatKey &keyFrame = channel->mRotationKeys[i];
        rotations.emplace_back(keyFrame.mValue, keyFrame.mTime);
    }

    scales.reserve(channel->mNumScalingKeys);
    for (int i = 0; i < channel->mNumScalingKeys; i++)
    {
        const aiVectorKey &keyFrame = channel->mScalingKeys[i];
        scales.emplace_back(keyFrame.mValue, keyFrame.mTime);
    }
}

_gapi_impl::Bone::Bone(Bone&& other) noexcept
: positions(std::move(other.positions))
, rotations(std::move(other.rotations))
, scales(std::move(other.scales))
, localTransform(std::exchange(other.localTransform, nexus::math::Mat4()))
, id(std::exchange(other.id, 0))
{ }

_gapi_impl::Bone& _gapi_impl::Bone::operator=(Bone&& other) noexcept
{
    if (this != &other)
    {
        positions = std::move(other.positions);
        rotations = std::move(other.rotations);
        scales = std::move(other.scales);

        localTransform = std::exchange(other.localTransform, nexus::math::Mat4());
        id = std::exchange(other.id, 0);
    }
    return *this;
}

math::Mat4 _gapi_impl::Bone::InterpolatePosition(float animationTime)
{
    if (positions.size() == 1)
    {
        return math::Mat4::Translate(positions[0].value);
    }

    int p0Index = GetPositionIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor = math::Normalize(animationTime,
        positions[p0Index].timeStamp, positions[p1Index].timeStamp);

    math::Vec3 finalPosition = math::Lerp(
        positions[p0Index].value,
        positions[p1Index].value,
        scaleFactor);

    return math::Mat4::Translate(finalPosition);
}

math::Mat4 _gapi_impl::Bone::InterpolateRotation(float animationTime)
{
    if (rotations.size() == 1)
    {
        return math::Mat4::FromQuaternion(rotations[0].value.Normalized());
    }

    int p0Index = GetRotationIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor = math::Normalize(animationTime,
        rotations[p0Index].timeStamp, rotations[p1Index].timeStamp);

    math::Vec4 finalRotation = math::SLerp(
        rotations[p0Index].value,
        rotations[p1Index].value,
        scaleFactor);

    return math::Mat4::FromQuaternion(finalRotation.Normalized());
}

math::Mat4 _gapi_impl::Bone::InterpolateScaling(float animationTime)
{
    if (scales.size() == 1)
    {
        return math::Mat4::Scale(scales[0].value);
    }

    int p0Index = GetScaleIndex(animationTime);
    int p1Index = p0Index + 1;

    float scaleFactor = math::Normalize(animationTime,
        scales[p0Index].timeStamp, scales[p1Index].timeStamp);

    math::Vec3 finalScale = math::Lerp(
        scales[p0Index].value,
        scales[p1Index].value,
        scaleFactor);

    return math::Mat4::Scale(finalScale);
}

int _gapi_impl::Bone::GetPositionIndex(float animationTime)
{
    for (int index = 0; index < positions.size() - 1; index++)
    {
        if (animationTime < positions[index + 1].timeStamp)
            return index;
    }
    assert(0); return 0;
}

int _gapi_impl::Bone::GetRotationIndex(float animationTime)
{
    for (int index = 0; index < rotations.size() - 1; index++)
    {
        if (animationTime < rotations[index + 1].timeStamp)
            return index;
    }
    assert(0); return 0;
}

int _gapi_impl::Bone::GetScaleIndex(float animationTime)
{
    for (int index = 0; index < scales.size() - 1; index++)
    {
        if (animationTime < scales[index + 1].timeStamp)
            return index;
    }
    assert(0); return 0;
}

void _gapi_impl::Bone::Update(float animationTime)
{
    math::Mat4 translation = InterpolatePosition(animationTime);
    math::Mat4 rotation = InterpolateRotation(animationTime);
    math::Mat4 scale = InterpolateScaling(animationTime);
    localTransform = scale * rotation * translation;
}
