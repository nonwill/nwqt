
NWQT_PVER_FIX=$${QT_MAJOR_VERSION}_$${ARCHITECTURE}
isEmpty(CUR_PRJ_DIR):CUR_PRJ_DIR = $$PWD/..
#message("CUR_PRJ_DIR = $${CUR_PRJ_DIR}" )

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
}

# This makes qmake generate translations

win32:# Windows doesn't seem to have *-qt4 symlinks
isEmpty(QMAKE_LRELEASE):QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
isEmpty(QMAKE_LRELEASE):QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease-qt4

# The *.qm files might not exist when qmake is run for the first time,
# causing the standard install rule to be ignored, and no translations
# will be installed. With this, we create the qm files during qmake run.
!win32 {
  system($${QMAKE_LRELEASE} -silent $${_PRO_FILE_} 2> /dev/null)
}
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_BASE}.qm
contains(TEMPLATE,lib) {
updateqm.commands = $$QMAKE_LRELEASE \
    ${QMAKE_FILE_IN} \
    -qm \
    $$DLLDESTDIR/lang/$$TARGET/${QMAKE_FILE_OUT}
} else {
updateqm.commands = $$QMAKE_LRELEASE \
    ${QMAKE_FILE_IN} \
    -qm \
    $$DESTDIR/lang/$$TARGET/${QMAKE_FILE_OUT}
}
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
TS_OUT = $$TRANSLATIONS
TS_OUT ~= s/.ts/.qm/g
PRE_TARGETDEPS += $$TS_OUT
