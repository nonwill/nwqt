#ifndef NWQT_APPLICATION_H
#define NWQT_APPLICATION_H

#include <QApplication>
#include <QMap>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
class NWQT_LIB NWQNativeEventFilter
{
public:
    NWQNativeEventFilter() {}
    virtual ~NWQNativeEventFilter() {}
    virtual bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) = 0;
};
#else
#include <QAbstractNativeEventFilter>
typedef class QAbstractNativeEventFilter NWQNativeEventFilter;
#endif

class NWQT_LIB NWQAppGui : public QApplication
{
    Q_OBJECT
public:
    NWQAppGui(int& argc, char** argv);
    virtual ~NWQAppGui();

    typedef QMap<QString/*Lang*/, QString/*locale*/> Languages;
    static Languages availableLanguages();
    static void addTransPath(const QString& dir);
    static void addTransCell(const QString& cell);
    static void setLanguage(const QString& locale);

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    void installNativeEventFilter(NWQNativeEventFilter* filterObj);
#ifdef Q_OS_WIN32
    bool winEventFilter(MSG* msg, long* result);
#endif
#endif
private:
    NWQNativeEventFilter* _nativeEvFilter;
private:
    typedef QMap<QString/*Cell*/, QTranslator*> Translators;
    static bool loadTranslator(Translators::iterator& it, const QString& locale);
private:
    static QStringList _transPaths;
    static Translators _transCells;
};

#endif
