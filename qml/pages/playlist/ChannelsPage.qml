import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.streamfish.Model 1.0
import "../../items/playlist"

Page {
    property alias playList: channelsModel.playList

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: qsTr("Channels")
            anchors { left: parent.left; top: parent.top; right: parent.right }
        }

        SearchField {
            id: searchField
            width: parent.width
            anchors.top: header.bottom
            inputMethodHints: Qt.ImhNoAutoUppercase | Qt.ImhNoPredictiveText
            labelVisible: false
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
                onClicked: pageStack.replace("../mediaplayer/MediaPlayerPage.qml", {channel: channel})
            }
        }
    }
}

