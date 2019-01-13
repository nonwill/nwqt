#include "nw_titlebar.h"
#include "nw_common_ui.h"
#include "nw_common.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QStyle>
#include <QEvent>
#include <QFile>

class NWQTitleBar::NWQTitleBarPriData
{
public:
    NWQTitleBarPriData() : isSysTitleBarUsed(false)
    { }
    ~NWQTitleBarPriData()
    { }

public:
    NWQSysLabel* lbTitle;
    QHBoxLayout* userToolbarHL_L;
    QHBoxLayout* userToolbarHL_R;
    QWidget* sysBtnbar;
    QAbstractButton* btn[NWQTitleBar::BTN_NULL];
    static int barHeight;
    //int btnHeight;
    static QSize btnSize;
    static QSize iconSize;
    bool isSysTitleBarUsed;
};

int NWQTitleBar::NWQTitleBarPriData::barHeight = -1;
QSize NWQTitleBar::NWQTitleBarPriData::btnSize;
QSize NWQTitleBar::NWQTitleBarPriData::iconSize;

NWQTitleBar::NWQTitleBar(QWidget* parent) : QWidget(parent), priData(new NWQTitleBarPriData())
{
    setAutoFillBackground(true);
    setContentsMargins(0, 0, 0, 0);
    setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum));
    if (NWQTitleBarPriData::barHeight <= 0)
    {
        NWQTitleBarPriData::barHeight = style()->pixelMetric(QStyle::PM_TitleBarHeight) - NWQCommon::imWidgetMargin * 2;
//#if (QT_VERSION < QT_VERSION_CHECK(5, 6, 0))
        int btnHeight = NWQTitleBarPriData::barHeight - NWQCommon::imWidgetMargin / 2;
        int iconHeight = NWQTitleBarPriData::barHeight - NWQCommon::imWidgetMargin;
//#else
//        int btnHeight = style()->pixelMetric(QStyle::PM_TitleBarButtonSize) - NWQCommon::imWidgetMargin * /*5 /*/ 2;
//        int iconHeight = style()->pixelMetric(QStyle::PM_TitleBarButtonIconSize) - NWQCommon::imWidgetMargin * 2;
//#endif
        setFixedHeight(NWQTitleBarPriData::barHeight);
        NWQTitleBarPriData::btnSize = QSize(btnHeight * 5 / 4, btnHeight);
        NWQTitleBarPriData::iconSize = QSize(iconHeight, iconHeight);
    }

    QHBoxLayout* layout = new QHBoxLayout();
    layout->setSpacing(NWQCommon::imWidgetMargin);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop | Qt::AlignBottom | Qt::AlignLeft);
    setLayout(layout);

    priData->btn[BTN_ICON]/*priData->btnSysIcon*/ = new NWQSysIconBtn(this);
    priData->btn[BTN_ICON]->setObjectName("imBtnSysIcon");
    priData->btn[BTN_ICON]->setFixedSize(priData->barHeight, priData->barHeight);
    priData->btn[BTN_ICON]->setIconSize(NWQTitleBarPriData::iconSize);

    priData->userToolbarHL_L = new QHBoxLayout();
    priData->userToolbarHL_L->setMargin(0);
    priData->userToolbarHL_L->setSpacing(0);
    priData->userToolbarHL_L->setContentsMargins(0, 0, 0, 0);

    priData->lbTitle = new NWQSysLabel(this);
    priData->lbTitle->setObjectName("imLbTitle");
    priData->lbTitle->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    priData->lbTitle->setFixedHeight(NWQTitleBarPriData::btnSize.height());

    priData->sysBtnbar = new QWidget(this);

    priData->btn[BTN_MIN]/*priData->btnMinimize*/ = new NWQSysBarBtn(priData->sysBtnbar);
    priData->btn[BTN_MIN]->setObjectName("imBtnMinimize");
    priData->btn[BTN_MIN]->setFixedSize(NWQTitleBarPriData::btnSize);
    priData->btn[BTN_MIN]->setIconSize(NWQTitleBarPriData::iconSize);
    //priData->btnMinimize->setIcon(style()->standardPixmap(QStyle::SP_TitleBarMinButton));
    connect(priData->btn[BTN_MIN], SIGNAL(clicked(bool)), parent, SLOT(showMinimized()));

    priData->btn[BTN_MAX]/*priData->btnMaximize*/ = new NWQSysBarBtn(priData->sysBtnbar);
    priData->btn[BTN_MAX]->setObjectName("imBtnMaximize");
    priData->btn[BTN_MAX]->setFixedSize(NWQTitleBarPriData::btnSize);
    priData->btn[BTN_MAX]->setIconSize(NWQTitleBarPriData::iconSize);
    priData->btn[BTN_MAX]->setCheckable(true);
    connect(priData->btn[BTN_MAX], SIGNAL(clicked(bool)), parent, SLOT(imShow()));
    connect(this, SIGNAL(signalMaximizeOrNormal()), priData->btn[BTN_MAX], SLOT(click()));

    priData->btn[BTN_CLOSE]/*priData->btnClose*/ = new NWQSysBarBtn(priData->sysBtnbar);
    priData->btn[BTN_CLOSE]->setObjectName("imBtnClose");
    priData->btn[BTN_CLOSE]->setFixedSize(NWQTitleBarPriData::btnSize);
    priData->btn[BTN_CLOSE]->setIconSize(NWQTitleBarPriData::iconSize);
    connect(priData->btn[BTN_CLOSE], SIGNAL(clicked(bool)), parent, SLOT(close()));

    priData->userToolbarHL_R = new QHBoxLayout();
    priData->userToolbarHL_R->setMargin(0);
    priData->userToolbarHL_R->setSpacing(0);
    priData->userToolbarHL_R->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout* sysBtnbarLayout = new QHBoxLayout();
    sysBtnbarLayout->setMargin(0);
    sysBtnbarLayout->setSpacing(0);
    sysBtnbarLayout->setContentsMargins(0, 0, 0, 0);
    priData->sysBtnbar->setLayout(sysBtnbarLayout);
    sysBtnbarLayout->addWidget(priData->btn[BTN_MIN]);
    sysBtnbarLayout->addWidget(priData->btn[BTN_MAX]);
    sysBtnbarLayout->addWidget(priData->btn[BTN_CLOSE]);

    layout->addWidget(priData->btn[BTN_ICON], 0, Qt::AlignLeft | Qt::AlignVCenter);
    layout->addLayout(priData->userToolbarHL_L);
    layout->addWidget(priData->lbTitle);
    layout->addLayout(priData->userToolbarHL_R);
    layout->addWidget(priData->sysBtnbar, 0, Qt::AlignRight | Qt::AlignVCenter);

    setIcon(BTN_ICON, QIcon(NWQCommon::iconBtnSys));
    setIcon(BTN_MIN, QIcon(NWQCommon::iconBtnMin));
    setIcon(BTN_MAX, QIcon(NWQCommon::iconBtnMax));
    setIcon(BTN_CLOSE, QIcon(NWQCommon::iconBtnClose));
    languageChanged();
}

NWQTitleBar::~NWQTitleBar()
{
    delete priData;
}

int NWQTitleBar::barHeight() const
{
    return NWQTitleBarPriData::barHeight;
}

int NWQTitleBar::btnHeight() const
{
    return NWQTitleBarPriData::btnSize.height();
}

int NWQTitleBar::btnWidth() const
{
    return NWQTitleBarPriData::btnSize.width();
}

const QSize& NWQTitleBar::btnSize() const
{
    return NWQTitleBarPriData::btnSize;
}

int NWQTitleBar::iconHeight() const
{
    return NWQTitleBarPriData::iconSize.height();
}

int NWQTitleBar::iconWidth() const
{
    return NWQTitleBarPriData::iconSize.width();
}

const QSize& NWQTitleBar::iconSize() const
{
    return NWQTitleBarPriData::iconSize;
}

void NWQTitleBar::useSystemTitlebar(bool use)
{
    if (use != priData->isSysTitleBarUsed)
    {
        priData->isSysTitleBarUsed = use;
        adjustBar();
    }
}

bool NWQTitleBar::systemTitlebarUsed()
{
    return priData->isSysTitleBarUsed;
}

void NWQTitleBar::adjustBar()
{
    if (systemTitlebarUsed())
    {
        if (priData->userToolbarHL_L->count() > 1 ||
            priData->userToolbarHL_R->count() > 1 ||
            qobject_cast<NWQSysIconBtn*>(priData->btn[BTN_ICON])->menu() != 0)
        {
            if (isHidden())
                show();
            priData->sysBtnbar->hide();
        }
        else
            hide();
    }
    else
    {
        if (!isHidden())
            show();
        if (priData->sysBtnbar->isHidden())
            priData->sysBtnbar->show();
    }
}

bool NWQTitleBar::isNWMaximized()
{
    return (priData->btn[BTN_MAX]->isChecked());
}

void NWQTitleBar::setIconMenu(QMenu* m)
{
    qobject_cast<NWQSysIconBtn*>(priData->btn[BTN_ICON])->setMenu(m);
}

QMenu* NWQTitleBar::iconMenu() const
{
    return qobject_cast<NWQSysIconBtn*>(priData->btn[BTN_ICON])->menu();
}

void NWQTitleBar::addLeftWidget(QWidget* w, int width, int stretch, Qt::Alignment alignment)
{
    w->setMaximumHeight(NWQTitleBarPriData::btnSize.height());
    if (width != 0)
        w->setFixedWidth(width);
    int index = priData->userToolbarHL_L->count();
    if (0 == index)
        priData->lbTitle->setAlignment(Qt::AlignCenter);
    priData->userToolbarHL_L->insertWidget(index, w, stretch, alignment);
    adjustBar();
}

void NWQTitleBar::removeLeftWidget(QWidget* w)
{
    priData->userToolbarHL_L->removeWidget(w);
    int index = priData->userToolbarHL_L->count();
    if (1 == index)
        priData->lbTitle->setAlignment(Qt::AlignTop | Qt::AlignBottom | Qt::AlignJustify);
    adjustBar();
}

void NWQTitleBar::addRightWidget(QWidget* w, int width, int stretch, Qt::Alignment alignment)
{
    w->setFixedHeight(NWQTitleBarPriData::btnSize.height());
    if (width != 0)
        w->setFixedWidth(width);
    priData->userToolbarHL_R->insertWidget(priData->userToolbarHL_R->count(), w, stretch, alignment);
    adjustBar();
}

void NWQTitleBar::removeRightWidget(QWidget* w)
{
    priData->userToolbarHL_R->removeWidget(w);
    adjustBar();
}

bool NWQTitleBar::event(QEvent* e)
{
    QEvent::Type t = e->type();
    switch (t)
    {
    case QEvent::MouseButtonDblClick:
        if (!priData->btn[BTN_MAX]->isHidden())
            emit signalMaximizeOrNormal();
        break;
    default:
        break;
    }

    return QWidget::event(e);
}

void NWQTitleBar::languageChanged()
{
    setToolTip(BTN_MIN, tr("Minimize"));
    if (priData->btn[BTN_MAX]->isChecked())
        setToolTip(BTN_MAX, tr("Nomal"));
    else
        setToolTip(BTN_MAX, tr("Maximize"));
    setToolTip(BTN_CLOSE, tr("Close"));
}

void NWQTitleBar::setIcon(const NWQT_BTN btn, const QIcon& ico)
{
    if (btn != BTN_NULL)
        priData->btn[btn]->setIcon(ico);
}

void NWQTitleBar::setToolTip(const NWQT_BTN btn, const QString& txt)
{
    if (btn != BTN_NULL)
        priData->btn[btn]->setToolTip(txt);
}

void NWQTitleBar::hideBtn(const NWQT_BTN btn)
{
    if (btn != BTN_NULL)
        priData->btn[btn]->hide();
}

void NWQTitleBar::showBtn(const NWQT_BTN btn)
{
    if (btn != BTN_NULL)
        priData->btn[btn]->show();
}

void NWQTitleBar::enableBtn(const NWQT_BTN btn, bool enabled)
{
    if (btn != BTN_NULL)
        priData->btn[btn]->setEnabled(enabled);
}

void NWQTitleBar::setTitle(const QString& txt)
{
    priData->lbTitle->setText(txt);
}

