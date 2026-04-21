import QtQuick 2.1
import Sailfish.Silica 1.0
import Nemo.Configuration 1.0
import 'pages'

ApplicationWindow {
    id: appWindow
    allowedOrientations: Orientation.All
    _defaultPageOrientations: Orientation.All
    cover: pageStack.currentPage && pageStack.currentPage.objectName == 'mediaPlayerPage' ? undefined : Qt.resolvedUrl("cover/CoverPage.qml")
    initialPage: config.openLastPlaylistOnStart ? lastPlaylistPageComponent : playlistsPageComponent

    Component {
        id: playlistsPageComponent
        PlaylistsPage {
            onPlaylistOpened:
                pageStack.push('pages/ChannelsPage.qml', {playlist: playlist})
        }
    }

    Component {
        id: lastPlaylistPageComponent
        ChannelsPage {
            id: channelsPage

            property bool openSettings

            Timer {
                // pageStack isn't ready in Component.onCompleted
                running: true
                interval: 0
                onTriggered: {
                    pageStack.completeAnimation()
                    var page = pageStack.pushAttached(Qt.resolvedUrl('pages/PlaylistsPage.qml'))
                    page.playlistOpened.connect(function (playlist) {
                        channelsPage.playlist = playlist
                        pageStack.navigateBack()
                    })
                    playlist = playlistsModel.lastOpenedPlaylist

                    if (openSettings) {
                        pageStack.navigateForward(PageStackAction.Immediate)
                        pageStack.push(Qt.resolvedUrl("pages/SettingsPage.qml"), null, PageStackAction.Immediate)
                    }
                }
            }
        }
    }

    Timer {
        id: pushSettingsTimer
        interval: 0
        onTriggered:
            pageStack.push(Qt.resolvedUrl("pages/SettingsPage.qml"), null, PageStackAction.Immediate)
    }

    function handleInitialPageChanged() {
        if (config.openLastPlaylistOnStart)
            pageStack.replaceAbove(null, lastPlaylistPageComponent, {openSettings: true}, PageStackAction.Immediate)
        else {
            pageStack.replaceAbove(null, playlistsPageComponent, null, PageStackAction.Immediate)
            pushSettingsTimer.start()
        }
    }

    ConfigurationGroup {
        id: config
        path: '/apps/harbour-streamfish'

        property bool replaceChannelsWithPlayer
        property bool preventDisplayBlanking: true
        property bool openLastPlaylistOnStart
    }
}
