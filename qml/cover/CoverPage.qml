import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge

        Image {
            source: "qrc:///res/streamfish.svg"
            width: Theme.iconSizeExtraLarge
            height: Theme.iconSizeExtraLarge
            sourceSize: Qt.size(width, height)
            anchors.horizontalCenter: parent.horizontalCenter
        }
        Label {
            text: qsTr("Streamfish")
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
