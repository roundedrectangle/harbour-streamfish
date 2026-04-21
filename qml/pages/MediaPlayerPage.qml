import QtQuick 2.1
import QtMultimedia 5.0
import Sailfish.Silica 1.0
import Nemo.KeepAlive 1.2

FullscreenContentPage {
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

        onIsPlayingChanged:
            if (overlay.active) {
                if (isPlaying)
                    hideOverlayTimer.restart()
                else
                    hideOverlayTimer.stop()
            }

        onHaveErrorChanged:
            if (haveError) stop()

        Rectangle {
            id: errorRectangle
            anchors.fill: parent
            color: Theme.rgba(palette.overlayBackgroundColor, Theme.opacityOverlay)
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

        MouseArea {
            anchors.fill: parent
            onClicked: overlay.active = !overlay.active
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
                interval: 5000
                onTriggered: overlay.active = false
            }
            onActiveChanged:
                if (active) {
                    if (video.isPlaying)
                        hideOverlayTimer.restart()
                } else
                    hideOverlayTimer.stop()

            Rectangle {
                width: parent.width
                height: header.height + Theme.itemSizeMedium
                gradient: Gradient {
                    GradientStop { position: 0.0; color: Theme.rgba(palette.overlayBackgroundColor, Theme.opacityOverlay) }
                    GradientStop { position: 1.0; color: 'transparent' }
                }
            }

            PageHeader {
                id: header
                title: channel.name
                titleColor: palette.primaryColor
            }

            IconButton {
                anchors.centerIn: parent
                width: Theme.iconSizeLarge
                height: Theme.iconSizeLarge
                icon.source: 'image://theme/icon-l-opaque-' + (video.isPlaying ? 'pause' : 'play')
                //icon.source: 'image://theme/icon-video-overlay-' + (video.isPlaying ? 'pause' : 'play')
                icon.color: undefined
                onClicked:
                    if (video.isPlaying) video.pause()
                    else video.play()
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: video.playbackState !== MediaPlayer.PlayingState && (video.status === MediaPlayer.Loading || video.status === MediaPlayer.Stalled)
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
