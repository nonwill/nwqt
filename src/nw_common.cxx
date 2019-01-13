#include "nw_common.h"
#include <QWidget>

const QColor  NWQCommon::activateBarColor =  QColor(102, 164, 226);
const QColor  NWQCommon::deactivateBarColor =  QColor(172, 183, 196);
const QString NWQCommon::iconBtnSys = ":/imtitlebar/default";
const QString NWQCommon::iconBtnMin = ":/imtitlebar/min";
const QString NWQCommon::iconBtnMax = ":/imtitlebar/max";
const QString NWQCommon::iconBtnNor = ":/imtitlebar/nor";
const QString NWQCommon::iconBtnClose = ":/imtitlebar/close";
const int     NWQCommon::imWidgetMargin =  2;

void NWQCommon::setBGColor(QWidget* w, bool isActivate)
{
    QPalette _palette = w->palette();
    _palette.setColor(QPalette::Window, NWQCommon::BGColor(isActivate));
    w->setPalette(_palette);
}

void NWQCommon::setBGColor(QWidget* w)
{
    QPalette _palette = w->palette();
    _palette.setColor(QPalette::Window, NWQCommon::BGColor(w->isActiveWindow()));
    w->setPalette(_palette);
}

const QColor& NWQCommon::BGColor(bool isActivate)
{
    return isActivate ? NWQCommon::activateBarColor : NWQCommon::deactivateBarColor;
}

const QColor& NWQCommon::BGColor(QWidget* w)
{
    return w->isActiveWindow() ? NWQCommon::activateBarColor : NWQCommon::deactivateBarColor;
}
