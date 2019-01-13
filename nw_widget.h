#ifndef NWQT_WIDGET_H
#define NWQT_WIDGET_H

#include <QWidget>

class NWQTitleBar;
class QMenu;

class NWQT_LIB NWQWidget: public QWidget
{
    Q_OBJECT
public:
    NWQWidget();
    ~NWQWidget();
    NWQTitleBar* titleBar() ;
    void setSystemTitleBar(bool flag = true);
    void setMinimizeToHide(bool minimizetoHide = false);
    bool isNWMaximized();

    void menuJoined(QMenu* menu);
    void menuLeft(QMenu* menu);

public slots:
    void setToplevel(bool tl = true);
    void setAutoHide(bool ah = true); // 使能，同时启用或停止
    void imShow();
    void onWidgetPopUp();
    void onWidgetPopEnd();

signals:
    void about();
    void signalMaximizeOrNormal();

protected slots:
    void beginAutoHide();
    void endAutoHide();
    void enableAutoHide(bool flag = true); // 停止或启用
    void delayedHide(int msec);
protected:
    QWidget* contentWidget();
    void setContentsMargins(int left, int top, int right, int bottom);
    bool isAutoHide() const;
    bool canImHidden() const;
    bool isImHidden() const;
    void updateWindowPositionType();
    bool event(QEvent* e);
    virtual void languageChanged();
    virtual void aboutToShow(const QWidget* const /*menu*/) {}
    virtual void aboutToHide(const QWidget* const /*menu*/) {}
    virtual void showMinimizeMessage(void) {}
private:
    void setLayout(QLayout*);
private:
    class NWQWidgetPriData;
    NWQWidgetPriData* priData;
};

#endif
