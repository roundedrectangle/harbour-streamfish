import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader { title: qsTr("Settings") }

            TextSwitch {
                text: qsTr("Go to playlists page when closing the media player")
                checked: config.replaceChannelsWithPlayer
                automaticCheck: false
                onClicked: config.replaceChannelsWithPlayer = !checked
            }

            TextSwitch {
                text: qsTr("Keep display active while media player is active")
                checked: config.preventDisplayBlanking
                automaticCheck: false
                onClicked: config.preventDisplayBlanking = !checked
            }
        }
    }
}
