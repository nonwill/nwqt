#include "nww_login.h"
#include "nw_titlebar.h"
#include "nw_common_ui.h"
#include "nw_common.h"
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QApplication>
#include <QShortcut>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>

IMLoginWidget::IMLoginWidget():
    NWQWidget()
{
    setWindowTitle(tr("Login"));
    setupUi();
    init();
    titleBar()->hideBtn(NWQTitleBar::BTN_MAX);
    adjustSize();
    setFixedSize(size());
}

void IMLoginWidget::setupUi()
{
    QFont font;
    font.setPointSize(10);
    QWidget* content = contentWidget();
    content->setFont(font);

    content->setFocusPolicy(Qt::TabFocus);
    QGridLayout* vbl = new QGridLayout;
    content->setLayout(vbl);

    QLabel* label = new QLabel(content);
    label->setText("User ID:");
    vbl->addWidget(label, 0, 0);

    ED_Fetion_ID = new QLineEdit(content);
    ED_Fetion_ID->setMaxLength(16);
    ED_Fetion_ID->setEchoMode(QLineEdit::Normal);
    vbl->addWidget(ED_Fetion_ID, 0, 1);

    loginState = new QComboBox(content);
    vbl->addWidget(loginState, 0, 2);

    QLabel* label_2 = new QLabel(content);
    label_2->setText("Password:");
    vbl->addWidget(label_2, 1, 0);

    ED_Fetion_Pwd = new QLineEdit(content);
    ED_Fetion_Pwd->setEchoMode(QLineEdit::Password);
    vbl->addWidget(ED_Fetion_Pwd, 1, 1);

    remPWD = new QCheckBox(content);
    remPWD->setText("Remember");
    vbl->addWidget(remPWD, 1, 2);

    QHBoxLayout* hl_login = new QHBoxLayout;
    vbl->addLayout(hl_login, 2, 0, 1, 3);

    Login_State = new QLabel(content);
    Login_State->setLayoutDirection(Qt::RightToLeft);
    Login_State->setText(" ");
    hl_login->addWidget(Login_State);

    BT_Login_Ok = new QLabelMouse(content);
    BT_Login_Ok->setEnabled(true);
    hl_login->addStretch();
    hl_login->addWidget(BT_Login_Ok);

    QWidget::setTabOrder(ED_Fetion_ID, ED_Fetion_Pwd);
    QWidget::setTabOrder(ED_Fetion_Pwd, loginState);
    QWidget::setTabOrder(loginState, remPWD);

    const QString qle_style = tr("QLineEdit {\n"
                                 "      border-width: 1px;\n"
                                 "      border-style: solid;\n"
                                 "      border-radius: 4px;\n"
                                 "  } ");
    ED_Fetion_Pwd->setStyleSheet(qle_style);
    ED_Fetion_ID->setStyleSheet(qle_style);
    BT_Login_Ok->setText("Login");
    loginState->clear();
    loginState->insertItems(0, QStringList()
                            << "online"
                            << "offline"
                            << "away"
                            << "busy"
                           );


}
IMLoginWidget::~IMLoginWidget()
{
}

void IMLoginWidget::Setting(bool)
{
    Login_State->setText("Settings changed! ");
}

void IMLoginWidget::about(bool)
{
    Login_State->setText("Test version 0.1.0");

}

void IMLoginWidget::slots_Login_Message(int message, qlonglong wParam, unsigned long /* lParam */)
{
    Login_State->setText(QObject::tr("message %1 wParam %2").arg(message).arg(wParam));
}


void IMLoginWidget::LoginOK()
{
    emit signal_LoginOK();
}

void IMLoginWidget::login()
{

    LoginOK();
}

void IMLoginWidget::init()
{
    QMenu* menu = new QMenu();
    QAction* ac_setting = menu->addAction("Setting");
    QAction* ac_about = menu->addAction("About");
    QAction* ac_sysbar = menu->addAction("Use SystemBar");
    ac_sysbar->setCheckable(true);
    titleBar()->setIconMenu(menu);

    QRegExp rx_port("[0-9]{0,11}");
    QValidator* validator_port = new QRegExpValidator(rx_port, this);
    ED_Fetion_ID->setValidator(validator_port);

    Login_State->setText("Test version 0.1.0");
    connect(this, SIGNAL(signal_Login_Message(int, qlonglong, unsigned long)),
            this, SLOT(slots_Login_Message(int, qlonglong, unsigned long)));
    connect(BT_Login_Ok, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(ac_setting, SIGNAL(triggered(bool)), this, SLOT(Setting(bool)));
    connect(ac_about, SIGNAL(triggered(bool)), this, SLOT(about(bool)));
    connect(ac_sysbar, SIGNAL(triggered(bool)), this, SLOT(useSystemTitleBar(bool)));
}

void IMLoginWidget::useSystemTitleBar(bool u)
{
    setSystemTitleBar(u);
}

void IMLoginWidget::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event);
    QApplication::quit();
    event->accept();
}


