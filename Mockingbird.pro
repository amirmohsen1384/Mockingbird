QT += core
QT += gui
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

RC_ICONS = icon.ico

SOURCES += \
    main.cpp \
    src/components/arrangewidget.cpp \
    src/components/filteredit.cpp \
    src/components/findwidget.cpp \
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
    src/dialogs/editor/abstractplaylisteditor.cpp \
    src/dialogs/editor/adminplaylisteditor.cpp \
    src/dialogs/editor/userplaylisteditor.cpp \
    src/dialogs/playlistplayer.cpp \
    src/dialogs/songedit.cpp \
    src/models/artistlistmodel.cpp \
    src/models/artistmodel.cpp \
    src/models/genremodel.cpp \
    src/models/maindelegate.cpp \
    src/models/playlistmodel.cpp \
    src/models/songmodel.cpp \
    src/models/songproxymodel.cpp \
    src/models/usermodel.cpp \
    src/panels/adminpanel.cpp \
    src/panels/userpanel.cpp \
    src/panels/welcomepanel.cpp \
    src/dialogs/storedialog.cpp

HEADERS += \
    include/components/arrangewidget.h \
    include/components/filteredit.h \
    include/components/findwidget.h \
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
    include/dialogs/editor/abstractplaylisteditor.h \
    include/dialogs/editor/adminplaylisteditor.h \
    include/dialogs/editor/userplaylisteditor.h \
    include/dialogs/playlistplayer.h \
    include/dialogs/songedit.h \
    include/models/artistlistmodel.h \
    include/models/artistmodel.h \
    include/models/genremodel.h \
    include/models/maindelegate.h \
    include/models/playlistmodel.h \
    include/models/songmodel.h \
    include/models/songproxymodel.h \
    include/models/usermodel.h \
    include/panels/adminpanel.h \
    include/panels/userpanel.h \
    include/panels/welcomepanel.h \
    include/dialogs/storedialog.h

FORMS += \
    ui/dialogs/storedialog.ui \
    ui/components/arrangewidget.ui \
    ui/components/filteredit.ui \
    ui/components/findwidget.ui \
    ui/components/player.ui \
    ui/dialogs/artistedit.ui \
    ui/dialogs/artistview.ui \
    ui/dialogs/editor/abstractplaylisteditor.ui \
    ui/dialogs/playlistplayer.ui \
    ui/dialogs/songedit.ui \
    ui/panels/adminpanel.ui \
    ui/panels/userpanel.ui \
    ui/panels/welcomepanel.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
