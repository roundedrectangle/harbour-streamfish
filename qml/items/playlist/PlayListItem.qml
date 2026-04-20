import QtQuick 2.1
import Sailfish.Silica 1.0

GridItem {
    property alias title: titleLabel.text
    property int channelsCount

    // SPDX-FileCopyrightText: 2016 - 2019 Jolla Ltd.
    // SPDX-FileCopyrightText: 2025 Jolla Mobile Ltd
    //
    // SPDX-License-Identifier: BSD-3-Clause
    Item {
        anchors.fill: parent
        clip: true
        Rectangle {
            rotation: 45
            x: parent.width - width
            width: parent.width * 1.412136
            height: parent.height * 1.412136
            gradient: Gradient {
                GradientStop { position: 0.0; color: Theme.rgba(Theme.primaryColor, 0) }
                GradientStop { position: 1.0; color: Theme.rgba(Theme.primaryColor, Theme.opacityFaint) }
            }
        }
    }

    Column {
        id: column
        x: Theme.paddingMedium
        width: parent.width - 2*x
        y: Theme.paddingMedium
        height: parent.height - 2*y
        spacing: Theme.paddingMedium

        Label {
            id: titleLabel
            width: parent.width
            font.pixelSize: Theme.fontSizeLarge
            wrapMode: Text.Wrap
        }

        Label {
            width: parent.width
            font.pixelSize: Theme.fontSizeMedium
            color: Theme.secondaryColor
            wrapMode: Text.Wrap
            text: qsTr("%Ln channels", '', channelsCount)
        }
    }
}
