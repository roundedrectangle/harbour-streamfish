import QtQuick 2.1
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import 'pages'

ApplicationWindow {
    id: mainwindow
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
    initialPage: Component { PlaylistsPage {} }

    ConfigurationGroup {
        id: config
        path: '/apps/harbour-streamfish'

        property bool replaceChannelsWithPlayer
        property bool preventDisplayBlanking: true
    }

    Connections {
        target: playlistsModel
        onPlaylistError:
            Notices.show(message)
    }

    cover: pageStack.currentPage.objectName == 'mediaPlayerPage' ? undefined : Qt.resolvedUrl("cover/CoverPage.qml")
}
