#-------------------------------------------------
#
# Project created by QtCreator 2018-06-15T08:58:27
#
#-------------------------------------------------
QT += core gui
TEMPLATE = lib
TARGET   = nwqt

CONFIG += build_nwqt
DEFINES += NWQT_LIB=Q_DECL_EXPORT
DEFINES += QT_DEPRECATED_WARNINGS
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

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
    include($$PWD/../3rdParty/qjson4/QJson4.pri)
    exists( $$(QTDIR)/include/QtGui/private/qwidgetresizehandler_p.h) {
        #message("QWidgetResizeHandler will be useed!" )
        DEFINES += QWIDGETRESIZEHANDLER_AVAILABLE
    }
}
DEFINES -= QWIDGETRESIZEHANDLER_AVAILABLE


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

include($$PWD/../../nwDeployed/nwDeployed.pri)
include($$PWD/../nwqt.pri)
