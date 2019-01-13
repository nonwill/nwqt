#ifndef NWQT_COMMON_H
#define NWQT_COMMON_H
#include <QColor>

class QWidget;

class NWQT_LIB NWQCommon
{
public:
    static void setBGColor(QWidget* w, bool isActivate);
    static void setBGColor(QWidget* w);
    static const QColor& BGColor(bool isActivate = true);
    static const QColor& BGColor(QWidget* w);
public:
    static const QColor activateBarColor;
    static const QColor deactivateBarColor;

    static const QString iconBtnSys;
    static const QString iconBtnMin;
    static const QString iconBtnMax;
    static const QString iconBtnNor;
    static const QString iconBtnClose;

    static const int     imWidgetMargin;
};

#endif
