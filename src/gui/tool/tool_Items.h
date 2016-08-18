#ifndef GUI_TOOL_ITEMS_H
#define GUI_TOOL_ITEMS_H

#include <functional>
#include <QButtonGroup>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include "util/Range.h"

namespace gui {
namespace tool {

//-------------------------------------------------------------------------------------------------
class SingleOutItem
{
public:
    SingleOutItem(int aButtonCount, const QSize& aButtonSize, QWidget* aParent);
    void setChoice(int aButtonIndex);
    void setToolTips(const QStringList& aTips);
    void setIcons(const QVector<QIcon*>& aIcons);
    void setIcons(const QVector<QIcon>& aIcons);
    void connect(const std::function<void(int)>& aPressed);
    int updateGeometry(const QPoint& aPos, int aWidth);

private:
    QButtonGroup* mGroup;
    QVector<QPushButton*> mButtons;
    QSize mButtonSize;
    int mButtonNum;
};

//-------------------------------------------------------------------------------------------------
class SliderItem
{
public:
    SliderItem(const QString& aLabel, const QPalette& aPalette, QWidget* aParent);
    void setAttribute(const util::Range& aRange, int aValue, int aPageStep, int aStep = 1);
    void connectOnChanged(const std::function<void(int)>& aValueChanged);
    void connectOnMoved(const std::function<void(int)>& aSliderMoved);
    int updateGeometry(const QPoint& aPos, int aWidth);
    void show() { mSlider->show(); mLabel->show(); }
    void hide() { mSlider->hide(); mLabel->hide(); }

private:
    void updateText(int aValue);
    QLabel* mLabel;
    QSlider* mSlider;
    QString mText;
};

} // namespace tool
} // namespace gui

#endif // GUI_TOOL_ITEMS_H
