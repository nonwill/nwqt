#include "nw_common_ui.h"
#include "nw_common.h"
#include <QMenu>
#include <QMouseEvent>
#include <QStylePainter>
#include <QStyleOption>

NWQSysLabel::NWQSysLabel(QWidget* parent) : QLabel(parent)
{
}

QString NWQSysLabel::text(void) const
{
    return _actualText;
}

void NWQSysLabel::setText(const QString& text)
{
    _actualText = text;
    update();
}

void NWQSysLabel::paintEvent(QPaintEvent*)
{
    QStylePainter p(this);
    QString elidedStr = p.fontMetrics().elidedText(_actualText, Qt::ElideRight, width(), alignment());
    p.drawText(rect(), alignment(), elidedStr);
}


NWQSysIconBtn::NWQSysIconBtn(QWidget* parent) : QPushButton(parent)
{
    setFlat(true);
    setStyleSheet("QPushButton{border:0px;}"
                  "QPushButton::menu-indicator{image:none;}");
}

NWQSysIconBtn::~NWQSysIconBtn()
{}

NWQSysBtn::NWQSysBtn(QWidget* parent) : QToolButton(parent)
{
    setPopupMode(QToolButton::InstantPopup);
    setToolButtonStyle(Qt::ToolButtonIconOnly);
    setStyleSheet("QToolButton{background:transparent;}");
}
NWQSysBtn::~NWQSysBtn()
{}

void NWQSysBtn::paintEvent(QPaintEvent*)
{
    QStylePainter p(this);
    QStyleOptionToolButton opt;
    initStyleOption(&opt);
    opt.features &= (~QStyleOptionToolButton::HasMenu);
    p.drawComplexControl(QStyle::CC_ToolButton, opt);
}

NWQSysBarBtn::NWQSysBarBtn(QWidget* parent) : NWQSysBtn(parent)
{
}

NWQSysBarBtn::~NWQSysBarBtn()
{}

bool NWQSysBarBtn::event(QEvent* e)
{
    dynamStyle(e->type());
    return NWQSysBtn::event(e);
}

void NWQSysBarBtn::dynamStyle(int event_type)
{
    switch (event_type)
    {
    case QEvent::ShowToParent:
    case QEvent::WindowDeactivate:
    case QEvent::WindowActivate:
    {
        const QString style("" //不显示下拉图片
                            "QToolButton{background-color:transparent;border:0px;}" // 按键本色
                            "QToolButton:hover{background-color:%1;border-style:outset;}"
                            "QToolButton:pressed{background-color:%3;border-style:outset;}");
        const QColor& bgcolor = NWQCommon::BGColor(isActiveWindow());
        setStyleSheet(style.arg(bgcolor.lighter(110).name()).arg(bgcolor.darker(110).name()));
        break;
    }
    default:
        break;
    }
}


void NWQMenuUpBtn::setMenu(QMenu* menu)
{
    _menu = menu;
}


void NWQMenuUpBtn::mouseReleaseEvent(QMouseEvent* e)
{
    NWQSysBtn::mouseReleaseEvent(e);
    if (e->button() == Qt::LeftButton)
    {
        if (_menu)
        {
            QPoint pos;
            pos.setX(0);
            pos.setY(0 - _menu->sizeHint().height());
            _menu->setNoReplayFor(this);
            _menu->exec(mapToGlobal(pos));
        }
    }
}
