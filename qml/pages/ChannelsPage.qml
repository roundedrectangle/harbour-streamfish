import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.streamfish.Model 1.0
import "../items/playlist"

Page {
    // If the playlist object is deleted while the channels page is open, segemntation fault might happen, but that's very unlikely
    property alias playlist: channelsModel.playlist

    Connections {
        target: playlist
        Component.onDestruction: pageStack.pop()
    }

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Channels")
        }

        SearchField {
            id: searchField
            width: parent.width
            height: visible ? implicitHeight : 0
            anchors.top: header.bottom
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            labelVisible: false
            visible: !!playlist
        }

        SilicaListView {
            width: parent.width
            anchors {
                top: searchField.bottom
                bottom: parent.bottom
            }
            clip: true

            model: M3UFilteredChannelsModel {
                id: channelsModel
                filter: searchField.text
            }

            delegate: ChannelItem {
                width: parent.width
                contentHeight: Theme.itemSizeSmall
                title: channel.name
                logoUrl: channel.logo
                onClicked:
                    (!config.openLastPlaylistOnStart && config.replaceChannelsWithPlayer ? pageStack.replace : pageStack.push)("MediaPlayerPage.qml", {channel: channel})
            }

            ViewPlaceholder {
                enabled: !playlist && config.openLastPlaylistOnStart
                text: qsTr("No playlist selected")
                hintText: qsTr("Swipe left to select a playlist")
            }
        }
    }
}

