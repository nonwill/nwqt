QT += core gui

greaterThan(QT_MAJOR_VERSION, 4) {
    QT += widgets
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}
contains(TARGET_ARCH, x86_64) {
    ARCHITECTURE = x64
} else {
    ARCHITECTURE = x86
}

isEmpty(CUR_PRJ_DIR):CUR_PRJ_DIR = $$PWD
message("CUR_PRJ_DIR = $${CUR_PRJ_DIR}" )
contains(TEMPLATE,lib) {
    DESTDIR = $${CUR_PRJ_DIR}/lib$${QT_MAJOR_VERSION}
    DLLDESTDIR = $${CUR_PRJ_DIR}/bin$${QT_MAJOR_VERSION}
} else {
    DESTDIR = $${CUR_PRJ_DIR}/bin$${QT_MAJOR_VERSION}
}

INCLUDEPATH += $$PWD
!contains(CONFIG,build_nwqt){
    DEFINES += NWQT_LIB=Q_DECL_IMPORT
    !greaterThan(QT_MAJOR_VERSION, 4){
        DEFINES += QJSON_EXPORT_DLL=Q_DECL_IMPORT
        INCLUDEPATH += $$PWD/3rdParty/qjson4
    }
    LIBS += -L$${PWD}/lib$${QT_MAJOR_VERSION}
    LIBS += -lnwqt
}

