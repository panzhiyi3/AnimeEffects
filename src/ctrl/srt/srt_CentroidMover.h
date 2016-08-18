#ifndef CTRL_SRT_CENTROIDMOVER_H
#define CTRL_SRT_CENTROIDMOVER_H

#include "cmnd/Stable.h"
#include "core/SRTKey.h"
#include "core/ObjectNode.h"

namespace ctrl {
namespace srt {

class CentroidMover : public cmnd::Stable
{
    struct KeyData { core::SRTKey* ptr; QVector3D prev; QVector3D next; };
    core::ObjectNode& mTarget;
    QVector2D mPrev;
    QVector2D mNext;
    QVector<KeyData> mKeys;
    QVector<KeyData> mChildKeys;
    bool mDone;
    bool mExecuteOnce;

public:
    CentroidMover(
            core::ObjectNode& aTarget,
            const QVector2D& aNext);

    void modifyValue(const QVector2D& aNext);
    virtual void exec();
    virtual void undo();
    virtual void redo();
};

} // namespace srt
} // namespace ctrl

#endif // CTRL_SRT_CENTROIDMOVER_H
