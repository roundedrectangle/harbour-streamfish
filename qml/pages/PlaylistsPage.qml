import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.streamfish.Model 1.0
import "../items/playlist"

Page {
    signal playlistOpened(var playlist)

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("About", "About the app")
                onClicked: pageStack.push("AboutPage.qml")
            }
            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push("SettingsPage.qml")
            }
            MenuItem {
                text: qsTr("Add")
                onClicked: pageStack.push("AddPlaylistPage.qml")
            }
        }

        PageHeader {
            id: header
            title: qsTr("Playlists")
        }

        SilicaGridView {
            id: listView
            width: parent.width
            anchors {
                top: header.bottom
                bottom: parent.bottom
            }
            cellWidth: width / Math.floor(width / Theme.itemSizeHuge)
            cellHeight: cellWidth

            model: playlistsModel

            delegate: PlayListItem {
                title: playlist.name
                channelsCount: playlist.channelsCount
                onClicked: {
                    playlistsModel.lastOpenedIndex = index
                    playlistOpened(playlist)
                }
                onRemoveRequested: playlistsModel.remove(index)
            }

            ViewPlaceholder {
                enabled: listView.count <= 0
                text: qsTr("Playlist is empty")
            }
        }
    }
}
