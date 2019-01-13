
# Input
INCLUDEPATH += $$PWD

contains(CONFIG,build_qjson4) {
    DEFINES += QJSON_EXPORT_DLL=Q_DECL_EXPORT
    HEADERS += $$PWD/QJsonArray.h        \
           $$PWD/QJsonDocument.h     \
           $$PWD/QJsonObject.h       \
           $$PWD/QJsonParseError.h   \
           $$PWD/QJsonValue.h        \
           $$PWD/QJsonValueRef.h     \
           $$PWD/QJsonParser.h       \
           $$PWD/QJsonRoot.h
    SOURCES += $$PWD/QJsonArray.cpp      \
           $$PWD/QJsonDocument.cpp   \
           $$PWD/QJsonObject.cpp     \
           $$PWD/QJsonParseError.cpp \
           $$PWD/QJsonValue.cpp      \
           $$PWD/QJsonValueRef.cpp   \
           $$PWD/QJsonParser.cpp
} else {
    DEFINES += QJSON_EXPORT_DLL=Q_DECL_IMPORT
}
