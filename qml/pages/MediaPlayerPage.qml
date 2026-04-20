import QtQuick 2.1
import QtMultimedia 5.0
import Sailfish.Silica 1.0
import Nemo.KeepAlive 1.2

Page {
    id: page
    objectName: 'mediaPlayerPage'
    allowedOrientations: Orientation.LandscapeMask

    property var channel

    DisplayBlanking {
        preventBlanking: config.preventDisplayBlanking && video.isPlaying
        // will automatically disable preventing on destruction
    }

    Video {
        id: video
        anchors.fill: parent
        source: channel.url
        autoPlay: true

        readonly property bool isPlaying: playbackState === MediaPlayer.PlayingState
        readonly property bool haveError: error !== MediaPlayer.NoError

        onPlaying:
            if (overlay.active)
                hideOverlayTimer.restart()

        onHaveErrorChanged:
            if (haveError) stop()

        Rectangle {
            id: errorRectangle
            anchors.fill: parent
            color: Theme.rgba(Theme.overlayBackgroundColor, Theme.opacityOverlay)
            visible: video.haveError

            Label {
                anchors.fill: parent
                font.pixelSize: Theme.fontSizeLarge
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                wrapMode: Text.WordWrap
                color: Theme.errorColor
                text: video.errorString
            }
        }

        Item {
            id: overlay
            property bool active

            anchors.fill: parent
            opacity: active ? 1 : 0
            enabled: active

            Behavior on opacity { FadeAnimator {} }

            Timer {
                id: hideOverlayTimer
                interval: 2000
                onTriggered: overlay.active = false
            }
            onActiveChanged:
                if (active && video.isPlaying) hideOverlayTimer.restart()

            Label {
                y: Theme.paddingLarge
                x: Theme.horizontalPageMargin
                width: parent.width - 2*x
                text: channel.name
                truncationMode: TruncationMode.Fade
            }

            IconButton {
                anchors.centerIn: parent
                icon.source: 'image://theme/icon-m-' + (video.isPlaying ? 'pause' : 'play')
                onClicked:
                    if (video.isPlaying) video.pause()
                    else video.play()
            }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: overlay.active = !overlay.active
        }

        BusyIndicator {
            id: busyIndicator
            anchors.centerIn: parent
            running: video.playbackState !== MediaPlayer.PlayingState && (status === MediaPlayer.Loading || status === MediaPlayer.Stalled)
            size: BusyIndicatorSize.Large
        }
    }

    CoverActionList { // Media player cover actions
        enabled: !video.haveError
        iconBackground: true

        CoverAction {
            iconSource: 'image://theme/icon-cover-' + (video.isPlaying ? 'pause' : 'play')
            onTriggered:
                if (video.isPlaying) video.pause()
                else video.play()
        }

        CoverAction {
            iconSource: 'image://theme/icon-cover-cancel'
            onTriggered: pageStack.pop()
        }
    }

    CoverActionList { // Fallback cover actions
        enabled: video.haveError
        iconBackground: true

        CoverAction {
            iconSource: "image://theme/icon-cover-cancel"
            onTriggered: pageStack.pop()
        }
    }
}
