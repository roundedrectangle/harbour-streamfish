import QtQuick 2.1
import Sailfish.Silica 1.0
import "pages/playlist"

ApplicationWindow {
    id: mainwindow
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
    initialPage: Component { PlaylistsPage {} }

    cover: pageStack.currentPage.objectName == 'mediaPlayerPage' ? undefined : Qt.resolvedUrl("cover/CoverPage.qml")
}
