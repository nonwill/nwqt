#ifndef IMW_LOGIN_H
#define IMW_LOGIN_H

#include "nw_widget.h"
#include <QTimer>
#include <QLabel>

class QLineEdit;
class QCheckBox;
class QComboBox;
class QLabelMouse;

class IMLoginWidget : public NWQWidget
{
    Q_OBJECT

public:
    IMLoginWidget();
    ~IMLoginWidget();

    void init();

    void LoginOK();
protected:
    void closeEvent(QCloseEvent* event);

private slots:
    void useSystemTitleBar(bool);
    void slots_Login_Message(int, qlonglong, unsigned long);
    void Setting(bool);
    void about(bool);

    void login();

signals:
    void signal_Login_Message(int, qlonglong, unsigned long);
    void signal_LoginOK();

private:
    void setupUi();

private:
    QLineEdit* ED_Fetion_Pwd;
    QLineEdit* ED_Fetion_ID;
    QCheckBox* remPWD;
    QLabelMouse* BT_Login_Ok;
    QComboBox* loginState;
    QLabel* Login_State;

};


class QLabelMouse : public QLabel
{
    Q_OBJECT
public:
    QLabelMouse(QWidget* parent) : QLabel(parent) { }

signals:
    void clicked();

protected:
    void mouseReleaseEvent(QMouseEvent*) { emit clicked(); }
};

#endif // IMW_LOGIN_H
