import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    objectName: 'settingsPage'

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader { title: qsTr("Settings") }

            TextSwitch {
                text: qsTr("Keep display active while media player is active")
                checked: config.preventDisplayBlanking
                automaticCheck: false
                onClicked: config.preventDisplayBlanking = !checked
            }

            TextSwitch {
                text: qsTr("Open last playlist on start")
                checked: config.openLastPlaylistOnStart
                automaticCheck: false
                onClicked: {
                    config.openLastPlaylistOnStart = !checked
                    appWindow.handleInitialPageChanged()
                }
            }

            TextSwitch {
                opacity: !config.openLastPlaylistOnStart ? 1 : 0
                height: !config.openLastPlaylistOnStart ? implicitHeight : 0

                Behavior on opacity { FadeAnimator {} }
                Behavior on height { NumberAnimation { duration: 200 } }

                text: qsTr("Go to playlists page when closing the media player")
                checked: config.replaceChannelsWithPlayer
                automaticCheck: false
                onClicked: config.replaceChannelsWithPlayer = !checked
            }
        }
    }
}
