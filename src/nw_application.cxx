#include "nw_application.h"
#include <QTranslator>
#include <QLocale>
#include <QDir>

#define NWQ_DEFAULT_LANG "en_US"

QStringList                 NWQAppGui::_transPaths;
NWQAppGui::Translators      NWQAppGui::_transCells;

NWQAppGui::NWQAppGui(int& argc, char** argv) : QApplication(argc, argv), _nativeEvFilter(0)
{
    Q_INIT_RESOURCE(nwqt);
    addTransPath(QApplication::applicationDirPath());
    addTransCell("nwqt");
}

NWQAppGui::~NWQAppGui()
{
    Q_CLEANUP_RESOURCE(nwqt);
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void NWQAppGui::installNativeEventFilter(NWQNativeEventFilter* filterObj)
{
    _nativeEvFilter = filterObj;
}

#ifdef Q_OS_WIN32
bool NWQAppGui::winEventFilter(MSG* msg, long* result)
{
    if (_nativeEvFilter)
        return _nativeEvFilter->nativeEventFilter(QByteArray("Q_OS_WIN32"), msg, result);
    return false;
}
#endif
#endif

bool NWQAppGui::loadTranslator(Translators::iterator& it, const QString& locale)
{
    foreach (QString path, _transPaths)
    {
        path += QDir::separator();
        if (!it.key().isEmpty() && it.key() != ".")
            path += (it.key() + QDir::separator());
        path += (locale + ".qm");
        QTranslator*& trans = it.value();
        QFileInfo file(path);
        if (!file.exists() || !file.isReadable())
            continue;
        if (!trans)
            trans = new QTranslator(instance());
        if (!trans->load(file.absoluteFilePath()))
        {
            delete trans;
            trans = 0;
            continue;
        }

        installTranslator(trans);
        return true;
    }
    return false;
}

void NWQAppGui::setLanguage(const QString& locale)
{
    for (Translators::iterator itrans(_transCells.begin()); itrans != _transCells.end(); ++itrans)
    {
        removeTranslator(itrans.value());
        if (!loadTranslator(itrans, locale))
            loadTranslator(itrans, NWQ_DEFAULT_LANG);
    }
}

void NWQAppGui::addTransPath(const QString& dir)
{
    if (_transPaths.contains(dir))
        return;
    _transPaths.push_back(dir);
}

void NWQAppGui::addTransCell(const QString& cell)
{
    if (_transCells.find(cell) != _transCells.end())
        return;
    _transCells[cell] = 0;
}

NWQAppGui::Languages NWQAppGui::availableLanguages()
{
    Languages _availLangs;
    foreach (const QString& cell, _transCells.keys())
    {
        foreach (QString path, _transPaths)
        {
            if (!cell.isEmpty() || cell != ".")
                path = path + QDir::separator() + cell;
            QFileInfoList entries = QDir(path).entryInfoList(QStringList() << "*_*.qm", QDir::Files | QDir::Readable);
            foreach (QFileInfo file, entries)
            {
                QStringList parts = file.baseName().split("_");
                QString locale = parts.at(parts.count() - 2).toLower() + "_" + parts.at(parts.count() - 1).toUpper();
                QString language = QLocale(locale).nativeLanguageName();//QLocale::languageToString(QLocale(locale).language());
                if (_availLangs.contains(language))
                    continue;
                _availLangs.insert(language, locale);
            }
        }
    }
    return _availLangs;
}
