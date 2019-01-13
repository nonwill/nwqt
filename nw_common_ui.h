#ifndef NWQT_COMMON_UI_H
#define NWQT_COMMON_UI_H
#include <QLabel>
#include <QPushButton>
#include <QToolButton>

class NWQT_LIB NWQSysLabel : public QLabel
{
public:
    explicit NWQSysLabel(QWidget* parent = 0);
    ~NWQSysLabel()
    {}

    QString text(void) const;
    void setText(const QString& text);
protected:
    void paintEvent(QPaintEvent*);

private:
    QString _actualText;
};

class NWQT_LIB NWQSysIconBtn : public QPushButton
{
    Q_OBJECT
public:
    explicit NWQSysIconBtn(QWidget* parent = 0) ;
    ~NWQSysIconBtn();
};

class NWQT_LIB NWQSysBtn : public QToolButton
{
    Q_OBJECT
public:
    explicit NWQSysBtn(QWidget* parent = 0);
    ~NWQSysBtn();

protected:
    virtual void paintEvent(QPaintEvent*);
};

class NWQT_LIB NWQSysBarBtn : public NWQSysBtn
{
    Q_OBJECT
public:
    explicit NWQSysBarBtn(QWidget* parent = 0);
    ~NWQSysBarBtn();

protected:
    virtual void dynamStyle(int event_type);
    virtual bool event(QEvent* e);
};

class NWQT_LIB NWQMenuUpBtn : public NWQSysBtn
{
    Q_OBJECT
public:
    explicit NWQMenuUpBtn(QWidget* parent = 0) :
        NWQSysBtn(parent), _menu(0)
    {}
    ~NWQMenuUpBtn()
    {}

    void setMenu(QMenu* menu);
protected:
    void mouseReleaseEvent(QMouseEvent* e);

private:
    QMenu* _menu;
};

#endif
