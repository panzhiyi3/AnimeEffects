#ifndef CORE_BONEKEY_H
#define CORE_BONEKEY_H

#include <memory>
#include <QList>
#include <QMatrix4x4>
#include "util/LinkPointer.h"
#include "core/TimeKey.h"
#include "core/Bone2.h"
#include "core/BoneInfluenceMap.h"
#include "core/ObjectNode.h"
namespace core { class Project; }

namespace core
{

class BoneKey : public TimeKey
{
public:
    class Data
    {
        QList<Bone2*> mTopBones;
    public:
        Data();
        Data(const Data& aRhs);
        Data& operator=(const Data& aRhs);
        ~Data();

        QList<Bone2*>& topBones();
        const QList<Bone2*>& topBones() const;
        void deleteAll();
    };

    class Cache
    {
        BoneInfluenceMap mInfluence;
        util::LinkPointer<ObjectNode> mNode;
        QMatrix4x4 mInnerMtx;
        Frame mFrameSign; // to debug that the cache updating be forgotten
    public:
        Cache();
        void setNode(ObjectNode& aNode);
        ObjectNode* node() { return mNode.get(); }
        const ObjectNode* node() const { return mNode.get(); }
        BoneInfluenceMap& influence() { return mInfluence; }
        const BoneInfluenceMap& influence() const { return mInfluence; }
        void setInnerMatrix(const QMatrix4x4& aMtx) { mInnerMtx = aMtx; }
        const QMatrix4x4& innerMatrix() const { return mInnerMtx; }
        void setFrameSign(const Frame& aFrame) { mFrameSign = aFrame; }
        const Frame& frameSign() const { return mFrameSign; }
    };
    typedef QList<Cache*> CacheList;

    BoneKey();
    ~BoneKey();

    Data& data() { return mData; }
    const Data& data() const { return mData; }

    ObjectNode* cacheOwner() const { return mCacheOwner.get(); }
    const CacheList& caches() const { return mCaches; }

    // reset cache list and rewrite influences
    void resetCaches(Project& aProject, ObjectNode& aOwner);
    // rewrite influences of unique roots specified
    void updateCaches(Project& aProject, ObjectNode& aOwner,
                      const QVector<ObjectNode*>& aUniqueRoots);

    virtual TimeKeyType type() const { return TimeKeyType_Bone; }
    virtual bool canHoldChild() const { return true; }
    virtual bool serialize(Serializer& aOut) const;
    virtual bool deserialize(Deserializer& aIn);

private:
    void updateCaches(Project& aProject, const QList<Cache*>& aTargets);
    Cache* popCache(ObjectNode& aNode);
    Cache* findCache(ObjectNode& aNode);
    void destroyCaches();
    bool serializeBone(Serializer& aOut, const Bone2* aBone) const;
    bool deserializeBone(Deserializer& aIn, Bone2* aBone);

    Data mData;
    CacheList mCaches;
    util::LinkPointer<ObjectNode> mCacheOwner;
};

} // namespace core

#endif // CORE_BONEKEY_H
