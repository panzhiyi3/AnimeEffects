#ifndef CORE_TIMEKEYEXPANS_H
#define CORE_TIMEKEYEXPANS_H

#include "core/SRTExpans.h"
#include "core/OpaKey.h"
#include "core/BoneKey.h"
#include "core/PoseKey.h"
#include "core/MeshKey.h"
#include "core/FFDKey.h"
#include "core/TimeKeyType.h"
#include "core/PosePalette.h"

namespace core
{

class TimeKeyExpans
{
public:
    TimeKeyExpans();

    // This value is valid when all of my children and I have caches.
    void setMasterCache(Frame aFrame);
    bool hasMasterCache(Frame aFrame) const;
    void clearMasterCache();

    void setKeyCache(TimeKeyType aType, Frame aFrame);
    bool hasKeyCache(TimeKeyType aType, Frame aFrame) const;

    void clearCaches();

    SRTExpans& srt() { return mSRT; }
    const SRTExpans& srt() const { return mSRT; }

    OpaKey::Data& opa() { return mOpa; }
    const OpaKey::Data& opa() const { return mOpa; }

    void setWorldOpacity(float aValue) { mWorldOpacity = aValue; }
    float worldOpacity() const { return mWorldOpacity; }

    void setAreaBone(BoneKey* aKey) { mAreaBone = aKey; }
    BoneKey* areaBone() { return mAreaBone; }
    const BoneKey* areaBone() const { return mAreaBone; }
    void setBoneInfluence(const BoneInfluenceMap* aMap) { mBoneInfluence = aMap; }
    const BoneInfluenceMap* boneInfluence() const { return mBoneInfluence; }
    void setOuterMatrix(const QMatrix4x4& aMtx) { mOuterMtx = aMtx; }
    const QMatrix4x4& outerMatrix() const { return mOuterMtx; }
    void setInnerMatrix(const QMatrix4x4& aMtx) { mInnerMtx = aMtx; }
    const QMatrix4x4& innerMatrix() const { return mInnerMtx; }
    void setBoneParent(const LayerMesh* aMesh) { mBoneParent = aMesh; }
    const LayerMesh* boneParent() const { return mBoneParent; }

    PoseKey::Data& pose() { return mPose; }
    const PoseKey::Data& pose() const { return mPose; }
    void setPoseParent(BoneKey* aKey) { mPoseParent = aKey; }
    BoneKey* poseParent() { return mPoseParent; }
    const BoneKey* poseParent() const { return mPoseParent; }
    PosePalette& posePalette() { return mPosePalette; }
    const PosePalette& posePalette() const { return mPosePalette; }

    void setAreaMesh(MeshKey* aKey) { mAreaMesh = aKey; }
    MeshKey* areaMesh() { return mAreaMesh; }

    FFDKey::Data& ffd() { return mFFD; }
    const FFDKey::Data& ffd() const { return mFFD; }

private:
    Frame mMasterCache;
    std::array<Frame, TimeKeyType_TERM> mKeyCaches;
    SRTExpans mSRT;
    OpaKey::Data mOpa;
    float mWorldOpacity;
    BoneKey* mAreaBone;
    const BoneInfluenceMap* mBoneInfluence;
    QMatrix4x4 mOuterMtx;
    QMatrix4x4 mInnerMtx;
    const LayerMesh* mBoneParent;
    PoseKey::Data mPose;
    BoneKey* mPoseParent;
    PosePalette mPosePalette;
    MeshKey* mAreaMesh;
    FFDKey::Data mFFD;
};

} // namespace core

#endif // CORE_TIMEKEYEXPANS_H
