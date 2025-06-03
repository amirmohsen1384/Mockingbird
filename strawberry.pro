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
    src/core/admin.cpp \
    src/core/artist.cpp \
    src/core/general.cpp \
    src/core/person.cpp \
    src/core/playlist.cpp \
    src/core/song.cpp \
    src/core/user.cpp \
    src/dialogs/artistedit.cpp \
    src/dialogs/artistview.cpp \
    src/dialogs/playlistedit.cpp \
    src/dialogs/playlistplayer.cpp \
    src/dialogs/songedit.cpp \
    src/models/artistlistmodel.cpp \
    src/models/artistmodel.cpp \
    src/models/genremodel.cpp \
    src/models/playlistmodel.cpp \
    src/models/songdelegate.cpp \
    src/pages/adminpage.cpp \
    src/pages/welcomepage.cpp \
    src/pages/userpanel.cpp

HEADERS += \
    include/components/imageview.h \
    include/components/player.h \
    include/core/admin.h \
    include/core/admin.h \
    include/core/artist.h \
    include/core/general.h \
    include/core/person.h \
    include/core/playlist.h \
    include/core/song.h \
    include/core/user.h \
    include/dialogs/artistedit.h \
    include/dialogs/artistview.h \
    include/dialogs/playlistedit.h \
    include/dialogs/playlistplayer.h \
    include/dialogs/songedit.h \
    include/models/artistlistmodel.h \
    include/models/artistmodel.h \
    include/models/genremodel.h \
    include/models/playlistmodel.h \
    include/models/songdelegate.h \
    include/pages/adminpage.h \
    include/pages/welcomepage.h \
    include/pages/userpanel.h

FORMS += \
    ui/components/player.ui \
    ui/dialogs/artistedit.ui \
    ui/dialogs/artistview.ui \
    ui/dialogs/playlistedit.ui \
    ui/dialogs/playlistplayer.ui \
    ui/dialogs/songedit.ui \
    ui/pages/adminpage.ui \
    ui/pages/welcomepage.ui \
    ui/pages/userpanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
