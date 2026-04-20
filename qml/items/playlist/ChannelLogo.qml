import QtQuick 2.1
import Sailfish.Silica 1.0
import "../../js/UrlHelper.js" as UrlHelper

Item {
    property string logoUrl
    property real contentWidth

    readonly property string channelNumber: {
        var cap = /([0-9]+)\.png/.exec(logoUrl)

        return (!cap || !cap[1]) ? '' : cap[1]
    }

    width: fallbackRectangle.visible ? fallbackRectangle.width : contentWidth

    Image {
        id: image
        asynchronous: true
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        visible: (status === Image.Ready)

        source: channelNumber.length > 0
                ? ''
                : (UrlHelper.isUrl(logoUrl) ? logoUrl : 'qrc:///res/channel.png')
    }

    Rectangle {
        id: fallbackRectangle
        color: Theme.secondaryHighlightColor
        height: parent.height + Theme.paddingMedium
        width: visible ? channelNumberLabel.contentWidth + Theme.paddingMedium : 0
        radius: width * 0.5
        visible: !image.visible

        Label {
            id: channelNumberLabel
            anchors.centerIn: parent
            font {
                pixelSize: Theme.fontSizeTiny
                bold: true
            }
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Theme.primaryColor
            text: channelNumber
        }
    }
}

