
greaterThan(QT_MAJOR_VERSION, 4) {
    TARGET_ARCH=$${QT_ARCH}
} else {
    TARGET_ARCH=$${QMAKE_HOST.arch}
}
contains(TARGET_ARCH, x86_64) {
    ARCHITECTURE = x64
} else {
    ARCHITECTURE = x86
}

NWQT_PVER_FIX=$${QT_MAJOR_VERSION}_$${ARCHITECTURE}
isEmpty(CUR_PRJ_DIR):CUR_PRJ_DIR = $$PWD/..
#message("CUR_PRJ_DIR = $${CUR_PRJ_DIR}" )
contains(TEMPLATE,lib) {
    DESTDIR = $${CUR_PRJ_DIR}/lib$${NWQT_PVER_FIX}
    DLLDESTDIR = $${CUR_PRJ_DIR}/bin$${NWQT_PVER_FIX}
} else {
    DESTDIR = $${CUR_PRJ_DIR}/bin$${NWQT_PVER_FIX}
}
LIBS += -L$${PWD}/../lib$${NWQT_PVER_FIX}

contains(QT, gui) {
    greaterThan(QT_MAJOR_VERSION, 4):QT += widgets
    INCLUDEPATH += $$PWD
    !contains(CONFIG,build_nwqt){
        DEFINES += NWQT_LIB=Q_DECL_IMPORT
        LIBS += -lnwqt
        !greaterThan(QT_MAJOR_VERSION, 4){
            DEFINES += QJSON_EXPORT_DLL=Q_DECL_IMPORT
            INCLUDEPATH += $$PWD/3rdParty/qjson4
        }
    }
#    DEFINES += FONTAWESOME_LIB=Q_DECL_IMPORT
#    INCLUDEPATH += $$PWD/3rdParty/QtAwesome
#    INCLUDEPATH += $$PWD/3rdParty/qt_eventdispatcher_libuv/src \
#                   $$PWD/3rdParty/qt_eventdispatcher_libuv/src-gui
#    DEFINES += QLIBUV_LIB=Q_DECL_IMPORT
#    INCLUDEPATH += $$PWD/3rdParty/libuv/include
#    LIBS += -L$$PWD/3rdParty/libuv/lib -luv
#    win32:win32-g++:LIBS += -lmswsock
#    win32:LIBS += -luserenv -lws2_32 -lpsapi -liphlpapi
}

contains(CONFIG,using_openssl){
    equals(ARCHITECTURE,x64) {
        INCLUDEPATH += $$(OPENSSL_DIR)/include64
        LIBS += -L$$(OPENSSL_DIR)/lib64
    } else {
        INCLUDEPATH += $$(OPENSSL_DIR)/include
        LIBS += -L$$(OPENSSL_DIR)/lib
    }
    TARGET_OS=$$(MSYSTEM)
    isEmpty(TARGET_OS) {
        win32:LIBS += -llibeay32
    }else{
        LIBS += -lcrypto
    }
}