QT += core
QT += gui
QT += multimedia
QT += multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    main.cpp \
    src/components/imageview.cpp \
    src/components/player.cpp \
    src/core/artist.cpp \
    src/core/general.cpp \
    src/core/playlist.cpp \
    src/core/song.cpp \
    src/dialogs/playlistplayer.cpp \
    src/dialogs/songedit.cpp \
    src/models/genremodel.cpp \
    src/models/playlistmodel.cpp \
    src/models/songdelegate.cpp \
    src/pages/welcomepage.cpp

HEADERS += \
    include/components/imageview.h \
    include/components/player.h \
    include/core/artist.h \
    include/core/general.h \
    include/core/playlist.h \
    include/core/song.h \
    include/dialogs/playlistplayer.h \
    include/dialogs/songedit.h \
    include/models/genremodel.h \
    include/models/playlistmodel.h \
    include/models/songdelegate.h \
    include/pages/welcomepage.h

FORMS += \
    ui/components/player.ui \
    ui/dialogs/playlistplayer.ui \
    ui/pages/welcomepage.ui \
    ui/dialogs/songedit.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
