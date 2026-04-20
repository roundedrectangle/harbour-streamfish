import QtQuick 2.1
import Sailfish.Silica 1.0
import "../js/UrlHelper.js" as UrlHelper

Dialog {
    canAccept: (nameField.text.length > 0) && UrlHelper.isUrl(urlField.text)
    onAccepted: playlistsModel.add(nameField.text, urlField.text)

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: content.height

        Column {
            id: content
            width: parent.width

            DialogHeader {
                acceptText: qsTr("Add")
            }

            TextField {
                id: nameField
                width: parent.width
                placeholderText: qsTr("Name")
            }

            TextField {
                id: urlField
                width: parent.width
                labelVisible: false
                placeholderText: qsTr("URL")
                inputMethodHints: Qt.ImhUrlCharactersOnly
            }
        }
    }
}

