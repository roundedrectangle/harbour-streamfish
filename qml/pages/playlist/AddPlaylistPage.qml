import QtQuick 2.1
import Sailfish.Silica 1.0
import harbour.streamfish.PlayList 1.0
import "../../models"
import "../../js/UrlHelper.js" as UrlHelper

Dialog {
    property Context context

    canAccept: (nameField.text.length > 0) && UrlHelper.isUrl(urlField.text)
    onAccepted: context.manager.add(nameField.text, urlField.text)

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
                placeholderText: qsTr("Url")
                inputMethodHints: Qt.ImhUrlCharactersOnly

                rightItem: IconButton {
                    onClicked: urlField.text = Clipboard.text

                    width: icon.width
                    height: icon.height
                    icon.source: "image://theme/icon-m-clipboard"
                }
            }
        }
    }
}

