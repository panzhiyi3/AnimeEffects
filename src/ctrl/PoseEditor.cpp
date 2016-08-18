#include "core/TimeKeyExpans.h"
#include "ctrl/PoseEditor.h"
#include "ctrl/bone/bone_Renderer.h"
#include "ctrl/pose/pose_TransBoneMode.h"

using namespace core;

namespace ctrl
{

PoseEditor::PoseEditor(Project& aProject)
    : mProject(aProject)
    , mTarget()
    , mKeyOwner()
    , mCurrent()
{
}

PoseEditor::~PoseEditor()
{
    finalize();
}

void PoseEditor::setTarget(core::ObjectNode* aTarget)
{
    finalize();

    if (!aTarget || !aTarget->timeLine()) return;

    mTarget.node = aTarget;
    resetCurrentTarget();
}

#if 0
void PoseEditor::updateParam(const PoseParam& aParam)
{
    const PoseParam prev = mParam;
    mParam = aParam;

    if (prev.mode != mParam.mode)
    {
        resetCurrentTarget();
    }
}
#endif

bool PoseEditor::updateCursor(const core::CameraInfo& aCamera, const core::AbstractCursor& aCursor)
{
    if (mCurrent)
    {
        return mCurrent->updateCursor(aCamera, aCursor);
    }
    return false;
}

void PoseEditor::updateEvent(EventType)
{
    resetCurrentTarget();
}

void PoseEditor::renderQt(const core::RenderInfo& aInfo, QPainter& aPainter)
{
    if (mTarget && mKeyOwner.key && mCurrent)
    {
        mCurrent->renderQt(aInfo, aPainter);
    }
}

void PoseEditor::finalize()
{
    mCurrent.reset();
    mKeyOwner.deleteOwnsKey();
    mTarget.clear();
}

void PoseEditor::resetCurrentTarget()
{
    mCurrent.reset();
    mKeyOwner.deleteOwnsKey();

    // read srt matrix
    if (mTarget)
    {
        XC_PTR_ASSERT(mTarget->timeLine());

        bool success = false;
        mTarget.mtx = mTarget->timeLine()->current().srt().worldMatrix();
        mTarget.invMtx = mTarget.mtx.inverted(&success);
        if (!success)
        {
            mTarget.node = nullptr;
        }
    }

    if (mTarget)
    {
        if (initializeKey(*mTarget->timeLine()))
        {
            mCurrent.reset(new pose::TransBoneMode(mProject, mTarget, mKeyOwner));
        }
    }
}

bool PoseEditor::initializeKey(TimeLine& aLine)
{
    const TimeLine::MapType& map = aLine.map(TimeKeyType_Pose);
    TimeKeyExpans& current = aLine.current();
    const int frame = mProject.animator().currentFrame().get();

    if (map.contains(frame))
    {
        // a key is exists
        mKeyOwner.key = static_cast<PoseKey*>(map.value(frame));
        XC_PTR_ASSERT(mKeyOwner.key);
        mKeyOwner.ownsKey = false;
        mKeyOwner.parent = mKeyOwner.key->parent();
        return true;
    }
    else if (current.areaBone())
    {
        // create new key
        mKeyOwner.key = new PoseKey();
        mKeyOwner.ownsKey = true;
        mKeyOwner.parent = current.areaBone();

        if (current.areaBone() == current.poseParent())
        {
            // copy blended pose
            mKeyOwner.key->data() = current.pose();
        }
        else
        {
            // create from original bones
            mKeyOwner.key->data().createBonesBy(*current.areaBone());
        }
        return true;
    }

    return false;
}

} // namespace ctrl
