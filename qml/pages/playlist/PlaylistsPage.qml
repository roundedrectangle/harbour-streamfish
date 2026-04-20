import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.streamfish.Model 1.0
import "../../items/playlist"

Page {
    id: playlistpage

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Add")
                onClicked: pageStack.push("AddPlaylistPage.qml", {context: mainwindow.context} )
            }
        }

        PageHeader {
            id: header
            title: qsTr("Playlists")
        }

        SilicaGridView {
            property real spacing: Theme.paddingSmall

            id: gvplaylist
            width: parent.width
            anchors {
                top: header.bottom
                bottom: parent.bottom
            }
            cellWidth: Math.min(width, height) / 3
            cellHeight: cellWidth

            model: M3UPlayListsModel {
                manager: mainwindow.context.manager
            }

            delegate: PlayListItem {
                title: playList.name
                channelsCount: playList.channelsCount
                onClicked: pageStack.push("ChannelsPage.qml", {playList: playList})
            }

            ViewPlaceholder {
                enabled: gvplaylist.count <= 0
                text: qsTr("Playlist is empty")
            }
        }
    }
}
