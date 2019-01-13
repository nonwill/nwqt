#ifndef NWQT_TITLEBAR_H
#define NWQT_TITLEBAR_H

#include <QWidget>

class QMenu;

class NWQT_LIB NWQTitleBar: public QWidget
{
    Q_OBJECT
public:
    enum NWQT_BTN { BTN_ICON = 0, BTN_MIN, BTN_MAX, BTN_CLOSE, BTN_NULL };
public:
    NWQTitleBar(QWidget* parent);
    ~NWQTitleBar();

    void setTitle(const QString& txt);
    void setIconMenu(QMenu* m);
    QMenu* iconMenu() const;

    void setIcon(const NWQT_BTN btn, const QIcon& ico);
    void setToolTip(const NWQT_BTN btn, const QString& txt);
    void hideBtn(const NWQT_BTN btn);
    void showBtn(const NWQT_BTN btn);
    void enableBtn(const NWQT_BTN btn, bool enabled = true);

    bool isNWMaximized();

    void useSystemTitlebar(bool use = false);
    bool systemTitlebarUsed();
    void adjustBar();

    void addRightWidget(QWidget* w, int width = 0, int stretch = 0, Qt::Alignment alignment = 0);
    void removeRightWidget(QWidget* w);

    void addLeftWidget(QWidget* w, int width = 0, int stretch = 0, Qt::Alignment alignment = 0);
    void removeLeftWidget(QWidget* w);

    int barHeight() const;
    int btnHeight() const;
    int btnWidth() const;
    const QSize& btnSize() const;
    int iconHeight() const;
    int iconWidth() const;
    const QSize& iconSize() const;
    void languageChanged();

signals:
    void signalMaximizeOrNormal();

protected:
    bool event(QEvent* e);

private:
    class NWQTitleBarPriData;
    NWQTitleBarPriData* priData;
};

#endif
