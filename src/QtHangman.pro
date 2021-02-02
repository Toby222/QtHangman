QT -= gui
QT += widgets

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x050f02    # disables all the APIs deprecated before Qt 5.15.2

SOURCES += \
        gallows.cpp \
        hangman.cpp \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    gallows.h \
    hangman.h \
    wordlist.h

RESOURCES += \
    words.qrc
