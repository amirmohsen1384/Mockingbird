QT += core
QT += gui
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    src/core/artist.cpp \
    src/core/general.cpp \
    src/core/playlist.cpp \
    src/core/song.cpp \
    src/dialogs/songedit.cpp \
    src/models/genremodel.cpp \
    src/pages/welcomepage.cpp

HEADERS += \
    include/core/artist.h \
    include/core/general.h \
    include/core/playlist.h \
    include/core/song.h \
    include/dialogs/songedit.h \
    include/models/genremodel.h \
    include/pages/welcomepage.h

FORMS += \
    ui/pages/welcomepage.ui \
    ui/dialogs/songedit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
