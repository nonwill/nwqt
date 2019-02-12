#include "nw_widget.h"
#include "nw_titlebar.h"
#include "nw_common.h"
#include <QMouseEvent>
#include <QIcon>
#include <QTimer>
#include <QVBoxLayout>
#include <QMenu>
#include <QDesktopWidget>
#include <QPainter>
#include <QBitmap>
#include <QApplication>
#ifdef QWIDGETRESIZEHANDLER_AVAILABLE
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <private/qwidgetresizehandler_p.h>
#else
#include <QtWidgets/private/qwidgetresizehandler_p.h>
#endif
#endif

#define AVAILABLEGEOMETRY_SIZE (QApplication::desktop()->availableGeometry().size())


class NWQWidget::NWQWidgetPriData
{
public:
    typedef uint8_t WidgetPos;
    enum WIDGET_POS : WidgetPos
    {
        WP_NORMAL = 0,
        WP_LEFT = 1,
        WP_RIGHT = 2,
        WP_TOP = 4,
        WP_BOTTOM = 8,
        WP_HIDDEN = 16,
    };
#ifndef QWIDGETRESIZEHANDLER_AVAILABLE
    typedef uint8_t GeometryStats;
    enum GeometryStat : GeometryStats
    {
        MovOrResize = 0x01,
        Moving = 0x02,
        ResizeLeft = 0x04,
        ResizeRight = 0x08,
        ResizeUp = 0x10,
        ResizeDown = 0x20,
        ResizeHor = ResizeLeft | ResizeRight,
        ResizeVer = ResizeUp | ResizeDown,
        ResizeUpLeft = ResizeLeft | ResizeUp,
        ResizeUpRight = ResizeRight | ResizeUp,
        ResizeDownLeft = ResizeLeft | ResizeDown,
        ResizeDownRight = ResizeRight | ResizeDown,
        Resize = ResizeLeft | ResizeRight | ResizeUp | ResizeDown
    };
    GeometryStats mode;
    QPoint lastPoint;
#else
    QWidgetResizeHandler* widgetResizeHandler;
#endif
    NWQTitleBar* titleBar;
    QWidget* contentWidget;
    QRect geometry;
    int overTimeAutoHide;
    WidgetPos positionState;
    bool minimizeToHide;
    bool autoHide;
    bool enableAutoHide;
    bool hideDelayed;

public:
    NWQWidgetPriData() ;
    ~NWQWidgetPriData();
    void drawBorder(NWQWidget* imw);
#ifndef QWIDGETRESIZEHANDLER_AVAILABLE
    GeometryStats updateDragCursor(NWQWidget* imw, const QPoint& p, bool updateCursor = true);
    bool isDragBlocked(const bool hor_enough, const bool ver_enough, const GeometryStats cur_mode);
    void resizeNWWidget(NWQWidget* imw, const QPoint& curPoint);
#endif
};

NWQWidget::NWQWidgetPriData::NWQWidgetPriData()
{
#ifndef QWIDGETRESIZEHANDLER_AVAILABLE
    mode = 0;
#else
    widgetResizeHandler = 0;
#endif
    minimizeToHide = false;
    autoHide = false;
    enableAutoHide = false;
    hideDelayed = false;
    overTimeAutoHide = 0;
}

NWQWidget::NWQWidgetPriData::~NWQWidgetPriData()
{
}

bool NWQWidget::isImHidden() const  {        return (priData->positionState & NWQWidgetPriData::WP_HIDDEN);    }
bool NWQWidget::canImHidden() const    {        return (priData->positionState != NWQWidgetPriData::WP_NORMAL);    }
bool NWQWidget::isAutoHide() const    {        return priData->autoHide;    }
NWQTitleBar* NWQWidget::titleBar()    {        return priData->titleBar;    }
QWidget* NWQWidget::contentWidget()    {        return priData->contentWidget;    }
void NWQWidget::setContentsMargins(int left, int top, int right, int bottom)
{
    priData->contentWidget->setContentsMargins(left, top, right, bottom);
}

const Qt::WindowFlags gwFlag = (Qt::Window | Qt::FramelessWindowHint |
                                Qt::WindowSystemMenuHint | Qt::WindowMinMaxButtonsHint);
NWQWidget::NWQWidget(/*QWidget* parent*/) : QWidget(0/*parent*/, gwFlag), priData(new NWQWidgetPriData())
{
    /*
     ___________________________________________________
     | _______________________________________________ |
     | |              titleBar                       | |
     | |icon | ... | title | ... | min | max | close | |
     | |_____________________________________________| |
     | ______________________________________________  |
     | |                                             | |
     | |                                             | |
     | |              contentWidget                  | |
     | |                                             | |
     | |                                             | |
     | |_____________________________________________| |
     |_________________________________________________|

     */
#ifdef QWIDGETRESIZEHANDLER_AVAILABLE
    priData->widgetResizeHandler = new QWidgetResizeHandler(this);
    priData->widgetResizeHandler->setMovingEnabled(true);
#else
    setAttribute(Qt::WA_Hover, true);
#endif
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setMargin(0);
    QWidget::setLayout(mainLayout);
    priData->titleBar = new NWQTitleBar(this);
    connect(this, SIGNAL(signalMaximizeOrNormal()), priData->titleBar, SIGNAL(signalMaximizeOrNormal()));
    mainLayout->addWidget(priData->titleBar);
    priData->contentWidget = new QWidget(this);
    priData->contentWidget->setContentsMargins(1, 0, 1, 1);
    mainLayout->addWidget(priData->contentWidget);

    QWidget::setContentsMargins(NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin);
    updateWindowPositionType();
}

NWQWidget::~NWQWidget()
{
    delete priData;
}

void NWQWidget::setSystemTitleBar(bool flag)
{
    const bool isvisible = isVisible();
    if (priData->titleBar->systemTitlebarUsed() == flag)
        return;

    priData->titleBar->useSystemTitlebar(flag);
    if (flag)
    {
        QWidget::setContentsMargins(0, 0, 0, 0);
#ifdef QWIDGETRESIZEHANDLER_AVAILABLE
        priData->widgetResizeHandler->setMovingEnabled(false);
#endif
        setWindowFlags(windowFlags() ^ Qt::FramelessWindowHint);
//        setWindowFlags(windowFlags() ^ Qt::WindowSystemMenuHint);
//        setWindowFlags(windowFlags() ^ Qt::WindowMinMaxButtonsHint);
        if (priData->autoHide)
            enableAutoHide(false);
    }
    else
    {
        QWidget::setContentsMargins(NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin);
#ifdef QWIDGETRESIZEHANDLER_AVAILABLE
        priData->widgetResizeHandler->setMovingEnabled(true);
#endif
        setWindowFlags(gwFlag);
        if (priData->autoHide)
            enableAutoHide(true);
    }

    if (isvisible)
    {
        showNormal();
    }
}

void NWQWidget::setMinimizeToHide(bool minimizetoHide)
{
    priData->minimizeToHide = minimizetoHide;
}

bool NWQWidget::isNWMaximized()
{
    return (priData->titleBar->isNWMaximized() || isMaximized() || isFullScreen());
}

#ifndef QWIDGETRESIZEHANDLER_AVAILABLE
bool NWQWidget::NWQWidgetPriData::isDragBlocked(const bool hor_enough, const bool ver_enough, const GeometryStats cur_mode)
{
    if (hor_enough || ver_enough)
    {
        const GeometryStats hor_m = mode & ResizeHor;
        const GeometryStats hor_cm = cur_mode & ResizeHor;
        const GeometryStats ver_m = mode & ResizeVer;
        const GeometryStats ver_cm = cur_mode & ResizeVer;
        if (hor_enough && ver_enough)
        {
            if (hor_m != hor_cm || ver_m != ver_cm)
                return true;
        }
        else if (hor_enough && !ver_enough)
        {
            if ((mode & ResizeHor) && hor_m != hor_cm)
                return true;
        }
        else /*if( ver_enough && !hor_enough)*/
        {
            if ((mode & ResizeVer) && ver_m != ver_cm)
                return true;
        }
    }
    return false;
}

NWQWidget::NWQWidgetPriData::GeometryStats NWQWidget::NWQWidgetPriData::updateDragCursor(NWQWidget* imw, const QPoint& p, bool updateCursor)
{
    if (imw->isNWMaximized())
        return 0;
    const QRect r = imw->rect();
    NWQWidgetPriData::GeometryStats mode = 0;
    int dif_p = p.x() - r.left();
    if (dif_p > -6 && dif_p < NWQCommon::imWidgetMargin)
        mode |= ResizeLeft;
    else
        mode &= ~ResizeLeft;
    dif_p = p.y() - r.top();
    if (dif_p > -6 && dif_p < NWQCommon::imWidgetMargin)
        mode |= ResizeUp;
    else
        mode &= ~ResizeUp;
    dif_p = p.x() - r.right();
    if (dif_p > -NWQCommon::imWidgetMargin && dif_p < 6)
        mode |= ResizeRight;
    else
        mode &= ~ResizeRight;
    dif_p = p.y() - r.bottom();
    if (dif_p > -(NWQCommon::imWidgetMargin) && dif_p < 6)
        mode |= ResizeDown;
    else
        mode &= ~ResizeDown;

    if (updateCursor)
    {
        if (mode & Resize)
        {
            if ((mode & ResizeUpRight) == ResizeUpRight || (mode & ResizeDownLeft) == ResizeDownLeft)
                imw->setCursor(Qt::SizeBDiagCursor);
            else if ((mode & ResizeUpLeft) == ResizeUpLeft || (mode & ResizeDownRight) == ResizeDownRight)
                imw->setCursor(Qt::SizeFDiagCursor);
            else if (mode & ResizeHor)
                imw->setCursor(Qt::SizeHorCursor);
            else /*if (mode & ResizeVer)*/
                imw->setCursor(Qt::SizeVerCursor);
        }
        else
        {
            imw->unsetCursor();
        }
    }
    return mode;
}

void NWQWidget::NWQWidgetPriData::resizeNWWidget(NWQWidget* imw, const QPoint& curPoint)
{
    const QSize maximumSize = imw->maximumSize();
    const QPoint displacement = lastPoint - curPoint;
    QPoint lastPos = imw->pos();
    QSize lastSize = imw->size();
    if (mode & ResizeUp)
    {
        lastSize.setHeight(imw->height() + displacement.y());
        if (lastSize.height() > maximumSize.height())
            lastPos.setY(imw->y() - displacement.y() + (lastSize.height() - maximumSize.height()));
        else
            lastPos.setY(imw->y() - displacement.y());
    }
    if (mode & ResizeLeft)
    {
        lastSize.setWidth(imw->width() + displacement.x());
        if (lastSize.width() > maximumSize.width())
            lastPos.setX(imw->x() - displacement.x() + (lastSize.width() - maximumSize.width()));
        else
            lastPos.setX(imw->x() - displacement.x());
    }

    if (mode & ResizeRight)
        lastSize.setWidth(imw->width() - displacement.x());
    if (mode & ResizeDown)
        lastSize.setHeight(imw->height() - displacement.y());

    if (lastSize.height() > maximumSize.height())
        lastSize.setHeight(maximumSize.height());
    if (lastSize.width() > maximumSize.width())
        lastSize.setWidth(maximumSize.width());

    imw->resize(lastSize);
    imw->updateGeometry();
    if ((mode & ResizeHor) && (mode & ResizeVer))
    {
        if (lastSize.width() < imw->minimumWidth())
            lastPos.setX(imw->pos().x());
        if (lastSize.height() < imw->minimumHeight())
            lastPos.setY(imw->pos().y());
    }
    if (((mode & ResizeLeft) && imw->width() != imw->minimumWidth()) ||
        ((mode & ResizeUp) && imw->height() != imw->minimumHeight()))
    {
        imw->move(lastPos);
    }
}
#endif

bool NWQWidget::event(QEvent* e)
{
    QEvent::Type t = e->type();
    switch (t)
    {
    case QEvent::Paint:
        if (!autoFillBackground() && !priData->titleBar->systemTitlebarUsed() && AVAILABLEGEOMETRY_SIZE != size())
            priData->drawBorder(this);
        break;
    case QEvent::Resize:
    {
        if (!priData->titleBar->systemTitlebarUsed() && AVAILABLEGEOMETRY_SIZE != size())
        {
            int w = width();
            int h = height();
            if (w && h)
            {
                QBitmap bmp(w, h);
                bmp.fill();
                QPainter pt(&bmp);
                pt.setRenderHint(QPainter::Antialiasing);
                pt.setPen(Qt::NoPen);
                pt.setBrush(Qt::black);
                pt.drawRoundedRect(0, 0, w/* - 1*/, h/* - 1*/, 2, 2);
                setMask(bmp);
                update();
            }
        }
        else if (!mask().isEmpty())
            clearMask();
        break;
    }
    case QEvent::Enter:
        if (0 != priData->overTimeAutoHide)
        {
            killTimer(priData->overTimeAutoHide);
            priData->overTimeAutoHide = 0;
            priData->hideDelayed = false;
        }
        endAutoHide();
        break;
    case QEvent::Leave:
        beginAutoHide();
        break;
    case QEvent::Move:
        if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized() && priData->enableAutoHide)
            updateWindowPositionType();
        break;
#ifndef QWIDGETRESIZEHANDLER_AVAILABLE
    case QEvent::MouseButtonPress:
        if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
        {
            QMouseEvent* event = dynamic_cast<QMouseEvent*>(e);
            if (event->buttons() & Qt::LeftButton)
            {
                if (!(priData->mode & NWQWidgetPriData::Resize))
                    priData->mode |= NWQWidgetPriData::Moving;
                priData->mode |= NWQWidgetPriData::MovOrResize;
                priData->lastPoint = event->globalPos();
            }
        }
        break;
    case QEvent::MouseButtonDblClick:
    case QEvent::MouseButtonRelease:
        priData->mode = 0;
        if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
        {
            priData->mode = priData->updateDragCursor(this, dynamic_cast<QMouseEvent*>(e)->pos());
        }
        break;
    case QEvent::MouseMove:
        if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
        {
            QMouseEvent* event = dynamic_cast<QMouseEvent*>(e);
            if ((event->buttons() & Qt::LeftButton) && (priData->mode & NWQWidgetPriData::MovOrResize))
            {
                if (priData->mode & NWQWidgetPriData::Resize)
                {
                    NWQWidgetPriData::GeometryStats cur_mode = priData->updateDragCursor(this, event->pos(), false);
                    bool w_blocked = width() == minimumWidth() || width() == maximumWidth();
                    bool h_blocked = height() == minimumHeight() || height() == maximumHeight();
                    if (!priData->isDragBlocked(w_blocked, h_blocked, cur_mode))
                        priData->resizeNWWidget(this, event->globalPos());
                }
                else if (priData->mode & NWQWidgetPriData::Moving)
                {
                    const QPoint displacement = priData->lastPoint - event->globalPos();
                    move(pos() - displacement);
                }
                priData->lastPoint = event->globalPos();
            }
        }
        break;
    case QEvent::HoverMove:
        if (0 != priData->overTimeAutoHide)
        {
            killTimer(priData->overTimeAutoHide);
            priData->overTimeAutoHide = 0;
            priData->hideDelayed = false;
        }
        if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
        {
            if (!(priData->mode & NWQWidgetPriData::MovOrResize) /*&& !isImHidden()*/)
                priData->mode = priData->updateDragCursor(this, (dynamic_cast<QHoverEvent*>(e))->pos());
        }
        break;
#endif
    case QEvent::Timer:
        if ((dynamic_cast<QTimerEvent*>(e))->timerId() == priData->overTimeAutoHide)
        {
            killTimer(priData->overTimeAutoHide);
            priData->overTimeAutoHide = 0;
            priData->hideDelayed = false;
            beginAutoHide();
            e->accept();
            return true;
        }
        break;
    case QEvent::Show:
        if ((dynamic_cast<QShowEvent*>(e))->spontaneous())
        {
            if (!priData->titleBar->systemTitlebarUsed())
            {
                if (!isMinimized())
                {
                    if (canImHidden())
                    {
                        QApplication::setActiveWindow(this);
                        endAutoHide();
                        priData->hideDelayed = true;
                        delayedHide(2500);
                    }
                }
            }
        }
        break;
    case QEvent::WindowStateChange:
    {
        const Qt::WindowStates stat = windowState();
        const Qt::WindowStates ostat = (dynamic_cast<QWindowStateChangeEvent*>(e))->oldState();
        if (!(ostat & Qt::WindowMinimized) && (stat & Qt::WindowMinimized))
        {
            if (priData->minimizeToHide)
            {
                QTimer::singleShot(0, this, SLOT(hide()));
                showMinimizeMessage();
            }
        }
        else if (priData->titleBar->isNWMaximized() &&
                 ((!(ostat & Qt::WindowMaximized) && (stat & Qt::WindowMaximized)) ||
                  (!(ostat & Qt::WindowFullScreen) && (stat & Qt::WindowFullScreen))))
        {
            priData->titleBar->setIcon(NWQTitleBar::BTN_MAX, QIcon(NWQCommon::iconBtnNor));
            priData->titleBar->setToolTip(NWQTitleBar::BTN_MAX, tr("Nomal"));
        }
        else if (!priData->titleBar->isNWMaximized() &&
                 (((ostat & Qt::WindowMaximized) || (ostat & Qt::WindowFullScreen)) &&
                  stat == Qt::WindowNoState))
        {
            priData->titleBar->setIcon(NWQTitleBar::BTN_MAX, QIcon(NWQCommon::iconBtnMax));
            priData->titleBar->setToolTip(NWQTitleBar::BTN_MAX, tr("Maximize"));
        }
        break;
    }
    case QEvent::WindowTitleChange:
        priData->titleBar->setTitle(windowTitle());
        break;
    case QEvent::WindowIconChange:
        priData->titleBar->setIcon(NWQTitleBar::BTN_ICON, windowIcon());
        break;
    case QEvent::WindowDeactivate:
        if (!autoFillBackground())
            NWQCommon::setBGColor(priData->titleBar, false);
        break;
    case QEvent::WindowActivate:
        if (!autoFillBackground())
            NWQCommon::setBGColor(priData->titleBar, true);
        break;
    case QEvent::LanguageChange:
        titleBar()->languageChanged();
        languageChanged();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void NWQWidget::languageChanged()
{
}

void NWQWidget::updateWindowPositionType()
{
    const QRect& rect = this->geometry();
    if (rect.top() <= 0)
        priData->positionState = NWQWidgetPriData::WP_TOP;
    else if (rect.bottom() >= QApplication::desktop()->height())
        priData->positionState = NWQWidgetPriData::WP_BOTTOM;
    else if (rect.left() <= 0)
        priData->positionState = NWQWidgetPriData::WP_LEFT;
    else if (rect.right() >=  QApplication::desktop()->width())
        priData->positionState = NWQWidgetPriData::WP_RIGHT;
    else
        priData->positionState = NWQWidgetPriData::WP_NORMAL;
}

void NWQWidget::menuJoined(QMenu* menu)
{
    connect(menu, SIGNAL(aboutToShow()), this, SLOT(onWidgetPopUp()));
    connect(menu, SIGNAL(aboutToHide()), this, SLOT(onWidgetPopEnd()));
}

void NWQWidget::menuLeft(QMenu* menu)
{
    disconnect(menu, SIGNAL(aboutToShow()), this, SLOT(onWidgetPopUp()));
    disconnect(menu, SIGNAL(aboutToHide()), this, SLOT(onWidgetPopEnd()));
}

void NWQWidget::onWidgetPopUp()
{
    if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
    {
        if (0 != priData->overTimeAutoHide)
        {
            killTimer(priData->overTimeAutoHide);
            priData->overTimeAutoHide = 0;
        }
        if (canImHidden())
            priData->hideDelayed = true;
    }
    QWidget* pW = qobject_cast<QWidget*>(sender());
    if (pW)
        aboutToShow(pW);
}

void NWQWidget::onWidgetPopEnd()
{
    QWidget* pW = qobject_cast<QWidget*>(sender());
    if (pW)
        aboutToHide(pW);
    if (!priData->titleBar->systemTitlebarUsed() && !isNWMaximized())
    {
        if (priData->enableAutoHide)
        {
            if (canImHidden())
                delayedHide(1000);
        }
    }
}

void NWQWidget::enableAutoHide(bool flag)
{
    if (priData->autoHide)
        priData->enableAutoHide = flag;
}

void NWQWidget::setToplevel(bool tl)
{
    if (tl)
        setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    else
        setWindowFlags(windowFlags() ^ Qt::WindowStaysOnTopHint);
}

void NWQWidget::setAutoHide(bool ah)
{
    if (ah)
    {
        if (!priData->autoHide)
        {
            priData->enableAutoHide =  priData->autoHide = true;
            setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
            if (!isVisible())
                show();
            updateWindowPositionType();
        }
    }
    else if (priData->autoHide)
    {
        if (isImHidden())
            endAutoHide();
        priData->enableAutoHide = priData->autoHide = false;
    }
}

void NWQWidget::delayedHide(int msec)
{
    if (0 != priData->overTimeAutoHide)
        killTimer(priData->overTimeAutoHide);
    priData->overTimeAutoHide = startTimer(msec);
}

void NWQWidget::beginAutoHide()
{
    if (priData->titleBar->systemTitlebarUsed() || isNWMaximized())
        return;
    if (!priData->enableAutoHide || priData->hideDelayed)
        return;

    updateWindowPositionType();
    const QRect& rect = this->geometry();
    if (priData->positionState == NWQWidgetPriData::WP_TOP || priData->positionState == NWQWidgetPriData::WP_BOTTOM)
    {
        int hold_x = 0;
        if (rect.left() < 0)
            hold_x = 0;
        else if (rect.right() >= QApplication::desktop()->width())
            hold_x =  QApplication::desktop()->width() - rect.width();
        else
            hold_x = rect.x();
        move(hold_x, priData->positionState == NWQWidgetPriData::WP_TOP ? (-rect.height() + NWQCommon::imWidgetMargin) : (QApplication::desktop()->height() - NWQCommon::imWidgetMargin));
    }
    else if (priData->positionState == NWQWidgetPriData::WP_LEFT)
        move(-rect.width() + NWQCommon::imWidgetMargin, rect.y());
    else if (priData->positionState == NWQWidgetPriData::WP_RIGHT)
        move(QApplication::desktop()->width() - NWQCommon::imWidgetMargin, rect.y());
    else
        return;

    priData->positionState |= NWQWidgetPriData::WP_HIDDEN;
}

void NWQWidget::endAutoHide()
{
    if (priData->titleBar->systemTitlebarUsed())
        return;
    if (!priData->enableAutoHide || priData->hideDelayed || (priData->positionState == NWQWidgetPriData::WP_NORMAL))
        return;

    const QRect& rect = geometry();
    if (rect.isNull())
        return;

    priData->positionState ^= NWQWidgetPriData::WP_HIDDEN;
    if (priData->positionState == NWQWidgetPriData::WP_TOP)
        move(rect.x(), -NWQCommon::imWidgetMargin);
    else if (priData->positionState == NWQWidgetPriData::WP_BOTTOM)
        move(rect.x(), QApplication::desktop()->height() - rect.height() + NWQCommon::imWidgetMargin);
    else if (priData->positionState == NWQWidgetPriData::WP_LEFT)
        move(-NWQCommon::imWidgetMargin, rect.y());
    else if (priData->positionState == NWQWidgetPriData::WP_RIGHT)
        move(QApplication::desktop()->width() - rect.width() + NWQCommon::imWidgetMargin, rect.y());
}

void NWQWidget::NWQWidgetPriData::drawBorder(NWQWidget* imw)
{
    QPainter pt(imw);
    pt.setRenderHint(QPainter::Antialiasing);

    QColor c1, c2, c3, c4, c5;
    if (imw->isActiveWindow())
    {
        c1 = QColor(32, 71, 110);
        c2 = QColor(52, 114, 176);
        c3 = QColor(65, 142, 220);
        c4 = QColor(59, 129, 200);
        c5 = QColor(102, 164, 226); //activate_color
    }
    else
    {
        c1 = QColor(78, 85, 93);
        c2 = QColor(132, 143, 156);
        c3 = QColor(157, 170, 186);
        c4 = QColor(142, 154, 169);
        c5 = QColor(172, 183, 196); //deactivate_color;
    }

    int w = imw->width();
    int h = imw->height();

    QPen pen1;
    pen1.setColor(c1);
    pen1.setWidth(1);
    pt.setPen(pen1);
    pt.setBrush(Qt::NoBrush);
    pt.drawRoundedRect(0, 0, w /*- 1*/, h /*- 1*/, 3, 3);

    QPen pen2;
    pen2.setColor(c2);
    pen2.setWidth(1);
    pt.setPen(pen2);
    pt.drawRoundedRect(1, 1, w - 2, h - 2, 2, 2);

    QPen pen3;
    pen3.setColor(c3);
    pen3.setWidth(1);
    pt.setPen(pen3);
    pt.drawRoundedRect(2, 2, w - 4, h - 4, 1, 1);

    QPen pen4;
    pen4.setColor(c4);
    pen4.setWidth(1);
    pt.setPen(pen4);
    pt.drawRoundedRect(3, 3, w - 6, h - 6, 1, 1);

    QPen pen5;
    pen5.setColor(c5);
    pen5.setWidth(1);
    pt.setPen(pen5);
    pt.drawLine(3, 2, 3, h);
    pt.drawLine(3, 3, w - 3, 3);
}

void NWQWidget::imShow()
{
    if (!priData->titleBar->isNWMaximized())
    {
        priData->titleBar->setIcon(NWQTitleBar::BTN_MAX, QIcon(NWQCommon::iconBtnMax));
        priData->titleBar->setToolTip(NWQTitleBar::BTN_MAX, tr("Maximize"));
        if (priData->titleBar->systemTitlebarUsed())
            showNormal();
        else
        {
            QWidget::setContentsMargins(NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin, NWQCommon::imWidgetMargin);
            setGeometry(priData->geometry);
            if (canImHidden())
            {
                endAutoHide();
                priData->hideDelayed = true;
                delayedHide(2500);
            }
        }
    }
    else
    {
        priData->titleBar->setIcon(NWQTitleBar::BTN_MAX, QIcon(NWQCommon::iconBtnNor));
        priData->titleBar->setToolTip(NWQTitleBar::BTN_MAX, tr("Nomal"));
        if (priData->titleBar->systemTitlebarUsed())
            showMaximized();
        else
        {
            priData->geometry = normalGeometry();
            move(0, 0);
            resize(AVAILABLEGEOMETRY_SIZE);
            if (AVAILABLEGEOMETRY_SIZE == size())
                QWidget::setContentsMargins(0, 0, 0, 0);
        }
    }
}

