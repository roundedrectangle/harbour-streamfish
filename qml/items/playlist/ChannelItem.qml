import QtQuick 2.1
import Sailfish.Silica 1.0
import "../../js/TagParser.js" as TagParser

ListItem {
    property string title
    property alias logoUrl: logo.logoUrl

    Row {
        x: Theme.paddingSmall
        width: parent.width - 2*x
        spacing: Theme.paddingMedium

        ChannelLogo {
            id: logo
            anchors.verticalCenter: parent.verticalCenter
            height: Theme.iconSizeSmall
            contentWidth: Theme.iconSizeSmall
        }

        Label {
            text: TagParser.parse(title)
            height: parent.height
            width: parent.width - logo.width
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
        }
    }
}
