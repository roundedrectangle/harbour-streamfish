# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-streamfish

CONFIG += sailfishapp c++11
QT += multimedia

SOURCES += src/harbour-streamfish.cpp \
    src/playlist/playlistsmodel.cpp \
    src/playlist/m3u/m3uparser.cpp \
    src/playlist/m3u/m3uplaylist.cpp \
    src/playlist/m3u/m3uchannel.cpp \
    src/model/m3uchannelsmodel.cpp \
    src/model/m3ufilteredchannelsmodel.cpp \
    src/utilities.cpp

OTHER_FILES += qml/harbour-streamfish.qml \
    qml/cover/CoverPage.qml \
    rpm/harbour-streamfish.changes.in \
    rpm/harbour-streamfish.spec \
    rpm/harbour-streamfish.yaml \
    translations/*.ts \
    harbour-streamfish.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# to disable building translations every time, comment out the
# following CONFIG line
CONFIG += sailfishapp_i18n

# German translation is enabled as an example. If you aren't
# planning to localize your app, remember to comment out the
# following TRANSLATIONS line. And also do not forget to
# modify the localized app name in the the .desktop file.
TRANSLATIONS += translations/harbour-streamfish-ru.ts \
    translations/harbour-streamfish-it.ts

DISTFILES += \
    qml/pages/playlist/AddPlaylistPage.qml \
    qml/models/AddedPlayListModel.qml \
    qml/items/playlist/PlayListItem.qml \
    qml/pages/playlist/ChannelsPage.qml \
    qml/pages/playlist/PlaylistsPage.qml \
    qml/items/playlist/ChannelItem.qml \
    qml/pages/mediaplayer/MediaPlayerPage.qml \
    qml/items/playlist/ChannelLogo.qml \
    qml/js/UrlHelper.js \
    qml/js/TagParser.js

HEADERS += \
    src/playlist/playlistsmodel.h \
    src/playlist/m3u/m3uplaylist.h \
    src/playlist/m3u/m3uparser.h \
    src/playlist/m3u/m3uchannel.h \
    src/model/m3uchannelsmodel.h \
    src/model/m3ufilteredchannelsmodel.h \
    src/utilities.h

RESOURCES += \
    resources.qrc

