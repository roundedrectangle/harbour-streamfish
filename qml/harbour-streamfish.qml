import QtQuick 2.1
import Sailfish.Silica 1.0
import "pages/playlist"
import "models"

ApplicationWindow {
    id: mainwindow
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
    initialPage: Component { PlaylistsPage { } }

    readonly property Context context: context

    Context {
        id: context
        Component.onCompleted: context.manager.load()
    }

    cover: pageStack.currentPage.objectName == 'mediaPlayerPage' ? undefined : Qt.resolvedUrl("cover/CoverPage.qml")
}
