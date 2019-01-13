#-------------------------------------------------
#
# Project created by QtCreator 2018-06-15T08:58:27
#
#-------------------------------------------------
TEMPLATE = lib
TARGET   = nwqt

CONFIG += build_nwqt

greaterThan(QT_MAJOR_VERSION, 4) {
    exists( $$(QTDIR)/include/QtWidgets/$${QT_VERSION}/QtWidgets/private/qwidgetresizehandler_p.h) {
        #message("QWidgetResizeHandler will be useed!" )
        DEFINES += QWIDGETRESIZEHANDLER_AVAILABLE
        INCLUDEPATH += $$(QTDIR)/include/QtWidgets/$${QT_VERSION} \
            $$(QTDIR)/include/QtGui/$${QT_VERSION} \
            $$(QTDIR)/include/QtCore/$${QT_VERSION}
    }
} else {
    CONFIG += build_qjson4
    exists( $$(QTDIR)/include/QtGui/private/qwidgetresizehandler_p.h) {
        #message("QWidgetResizeHandler will be useed!" )
        DEFINES += QWIDGETRESIZEHANDLER_AVAILABLE
    }
}
DEFINES -= QWIDGETRESIZEHANDLER_AVAILABLE

include($${PWD}/../build.pri)
SOURCES += \
#    im_systemtray.cxx \
#    imw_common_ui.cxx
    nw_application.cxx \
    nw_common.cxx \
    nw_common_ui.cxx \
    nw_titlebar.cxx \
    nw_widget.cxx

HEADERS += \
    ../nw_widget.h \
#    $${PWD}/../im_systemtray.h \
#    $${PWD}/../imw_common_ui.h
    ../nw_titlebar.h \
    ../nw_common_ui.h \
    ../nw_common.h \
    ../nw_application.h

TRANSLATIONS += en_US.ts zh_CN.ts

RESOURCES += $${PWD}/../nwqt.qrc
