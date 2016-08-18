#include "util/TreeUtil.h"
#include "core/ObjectTreeNotifier.h"
#include "core/Project.h"

namespace core
{

ObjectTreeNotifier::ObjectTreeNotifier(Project& aProject)
    : mEvent(aProject)
{
}

void ObjectTreeNotifier::onExecuted()
{
    auto& roots = mEvent.roots();

    // push moved root
    {
        for (auto target : mEvent.targets())
        {
            XC_PTR_ASSERT(target.node);

            // removed root
            if (target.parent && !roots.contains(target.parent))
            {
                roots.push_back(target.parent);
            }

            // inserted root
            if (target.node->parent() && !roots.contains(target.node->parent()))
            {
                roots.push_back(target.node->parent());
            }
        }
    }

    // remove duplicated root
    for (auto itr = roots.begin(); itr != roots.end();)
    {
        bool remove = false;
        auto node = *itr;
        auto nodepos = util::TreeUtil::getTreePos(node);
        XC_ASSERT(nodepos.isValid());

        // check duplication
        for (auto check : roots)
        {
            if (node == check) continue;

            auto checkpos = util::TreeUtil::getTreePos(check);
            XC_ASSERT(checkpos.isValid());

            if (checkpos.depth() <= nodepos.depth())
            {
                remove = true;
                for (int i = 0; i < checkpos.depth(); ++i)
                {
                    if (checkpos.row(i) != nodepos.row(i))
                    {
                        remove = false;
                        break;
                    }
                }
            }
            if (remove) break;
        }

        itr = remove ? roots.erase(itr) : (itr + 1);
    }

#if 0
    qDebug() << "move node";
    for (auto root : roots) qDebug() << root->name();
#endif

    onRedone();
}

void ObjectTreeNotifier::onUndone()
{
    mEvent.project().onTreeRestructured(mEvent, true);
}

void ObjectTreeNotifier::onRedone()
{
    mEvent.project().onTreeRestructured(mEvent, false);
}

} // namespace core
