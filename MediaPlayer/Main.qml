// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Window
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import QtMultimedia
import QtQuick.Effects
import MediaControls
import Config

ApplicationWindow {
    id: root
    property string currentPage: ""
    objectName: "appWindow"
    width: 1200
    height: 780
    minimumHeight: 460
    minimumWidth: 640
    visible: true
    color: Config.mainColor
    title: qsTr("Multimedia Player")
    required property url source
    required property list<string> nameFilters
    required property int selectedNameFilter

    property alias currentFile: playlistInfo.currentIndex
    property alias playlistLooped: playbackControl.isPlaylistLooped
    property alias metadataInfo: settingsInfo.metadataInfo
    property alias tracksInfo: settingsInfo.tracksInfo

    function playMedia() {
        mediaPlayer.source = playlistInfo.getSource();
        mediaPlayer.play();
    }

    function closeOverlays() {
        settingsInfo.visible = false;
        playlistInfo.visible = false;
    }

    function showOverlay(overlay) {
        closeOverlays();
        overlay.visible = true;
    }

    function openFile(path) {
        ++currentFile;
        playlistInfo.addFile(currentFile, path);
        mediaPlayer.source = path;
        mediaPlayer.play();
    }

    // function switchPage(pageId) {
    //     if (pageId === currentPage)
    //         return;
    //     currentPage = pageId;

    //     // reset pages
    //     playingPage.visible = false;
    //     libraryPage.visible = false;

    //     // enable the correct page
    //     switch (pageId) {
    //     case "playing":
    //         playingPage.visible = true;
    //         break;
    //     case "library":
    //         libraryPage.visible = true;
    //         break;
    //     }

    //     // Perform window action (e.g., stackView.replace(pageId))
    // }

    NavBar {
        id: navbar
        anchors.top: parent.bottom
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        width: 200
        onPageRequested: pageUrl => stackView.replace(pageUrl)
    }
    Rectangle {
        id: rectangleRoot
        anchors.left: navbar.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        color: "#221c31"
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
            onPositionChanged: {
                if (!seeker.opacity) {
                    if (videoOutput.fullScreen) {
                        showControls.start();
                    } else {
                        seeker.showSeeker.start();
                    }
                } else {
                    timer.restart();
                }
            }
            onClicked: root.closeOverlays()
        }

        Timer {
            id: timer
            interval: 3000
            onTriggered: {
                if (!seeker.isMediaSliderPressed) {
                    if (videoOutput.fullScreen) {
                        hideControls.start();
                    } else {
                        seeker.hideSeeker.start();
                    }
                } else {
                    timer.restart();
                }
            }
        }

        ErrorPopup {
            id: errorPopup
        }

        TouchMenu {
            id: menuPopup
            x: (parent.width - width) / 2
            y: parent.height - height - 32
            width: root.width - 64
            openFileMenuItem.onClicked: {
                menuPopup.close();
                menuBar.openFileMenu.open();
            }

            openUrlMenuItem.onClicked: {
                menuPopup.close();
                menuBar.openUrlPopup.open();
            }
        }

        MediaDevices {
            id: mediaDevices
            onAudioOutputsChanged: {
                mediaPlayer.audioOutput.device = defaultAudioOutput;
            }
        }

        MediaPlayer {
            id: mediaPlayer

            playbackRate: playbackControl.playbackRate
            videoOutput: videoOutput
            audioOutput: AudioOutput {
                id: audio
                volume: playbackControl.volume
            }
            source: new URL("https://download.qt.io/learning/videos/media-player-example/Qt_LogoMergeEffect.mp4")

            function updateMetadata() {
                root.metadataInfo.clear();
                root.metadataInfo.read(mediaPlayer.metaData);
            }

            onMetaDataChanged: updateMetadata()
            onActiveTracksChanged: updateMetadata()
            onErrorOccurred: {
                errorPopup.errorMsg = mediaPlayer.errorString;
                errorPopup.open();
            }
            onTracksChanged: {
                settingsInfo.tracksInfo.selectedAudioTrack = mediaPlayer.activeAudioTrack;
                settingsInfo.tracksInfo.selectedVideoTrack = mediaPlayer.activeVideoTrack;
                settingsInfo.tracksInfo.selectedSubtitleTrack = mediaPlayer.activeSubtitleTrack;
                updateMetadata();
            }

            onMediaStatusChanged: {
                if ((MediaPlayer.EndOfMedia === mediaStatus && mediaPlayer.loops !== MediaPlayer.Infinite) && ((root.currentFile < playlistInfo.mediaCount - 1) || playlistInfo.isShuffled)) {
                    if (!playlistInfo.isShuffled) {
                        ++root.currentFile;
                    }
                    root.playMedia();
                } else if (MediaPlayer.EndOfMedia === mediaStatus && root.playlistLooped && playlistInfo.mediaCount) {
                    root.currentFile = 0;
                    root.playMedia();
                }
            }
        }

        ColumnLayout {
            anchors.fill: parent
            spacing: 0
            StackView {
                id: stackView
                Layout.fillWidth: true
                Layout.fillHeight: true
                // Initial default page loaded on app launch
                initialItem: "LibraryPage.qml"

                // Optional: Disable transition animations for tab-like switching
                replaceEnter: Transition {
                    NumberAnimation {
                        property: "opacity"
                        from: 0
                        to: 1
                        duration: 150
                    }
                }
                replaceExit: Transition {
                    NumberAnimation {
                        property: "opacity"
                        from: 1
                        to: 0
                        duration: 150
                    }
                }
            }

            PlaybackSeekControl {
                id: seeker
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: playbackControl.top
                mediaPlayer: mediaPlayer

                // fullScreenButton.onClicked: {
                //     // if (mediaPlayer.hasVideo) {
                //     // }
                //     videoOutput.fullScreen ? root.showNormal() : root.showFullScreen();
                //     videoOutput.fullScreen = !videoOutput.fullScreen;
                //     covertArt.visible = !covertArt.visible
                // }

                settingsButton.onClicked: !settingsInfo.visible ? root.showOverlay(settingsInfo) : root.closeOverlays()
            }

            PlaybackControl {
                id: playbackControl

                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right

                mediaPlayer: mediaPlayer
                isPlaylistVisible: playlistInfo.visible

                onPlayNextFile: {
                    if (playlistInfo.mediaCount) {
                        if (!playlistInfo.isShuffled) {
                            ++root.currentFile;
                            if (root.currentFile > playlistInfo.mediaCount - 1 && root.playlistLooped) {
                                root.currentFile = 0;
                            } else if (root.currentFile > playlistInfo.mediaCount - 1 && !root.playlistLooped) {
                                --root.currentFile;
                                return;
                            }
                        }
                        root.playMedia();
                    }
                }

                onPlayPreviousFile: {
                    if (playlistInfo.mediaCount) {
                        if (!playlistInfo.isShuffled) {
                            --root.currentFile;
                            if (root.currentFile < 0 && isPlaylistLooped) {
                                root.currentFile = playlistInfo.mediaCount - 1;
                            } else if (root.currentFile < 0 && !root.playlistLooped) {
                                ++root.currentFile;
                                return;
                            }
                        }
                        root.playMedia();
                    }
                }

                playlistButton.onClicked: !playlistInfo.visible ? root.showOverlay(playlistInfo) : root.closeOverlays()
                menuButton.onClicked: menuPopup.open()
            }
        }

        MultiEffect {
            source: settingsInfo
            anchors.fill: settingsInfo
            shadowEnabled: settingsInfo.visible || playlistInfo.visible
            visible: settingsInfo.visible || playlistInfo.visible
        }

        PlaylistInfo {
            id: playlistInfo

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: seeker.opacity ? seeker.top : playbackControl.top
            anchors.topMargin: 10
            anchors.rightMargin: 5

            visible: false
            isShuffled: playbackControl.isPlaylistShuffled

            onPlaylistUpdated: {
                if (mediaPlayer.playbackState == MediaPlayer.StoppedState && root.currentFile < playlistInfo.mediaCount - 1) {
                    ++root.currentFile;
                    root.playMedia();
                }
            }

            onCurrentFileRemoved: {
                mediaPlayer.stop();
                if (root.currentFile < playlistInfo.mediaCount - 1) {
                    root.playMedia();
                } else if (playlistInfo.mediaCount) {
                    --root.currentFile;
                    root.playMedia();
                }
            }
        }

        SettingsInfo {
            id: settingsInfo

            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: seeker.opacity ? seeker.top : playbackControl.top
            anchors.topMargin: 10
            anchors.rightMargin: 5

            mediaPlayer: mediaPlayer
            selectedAudioTrack: mediaPlayer.activeAudioTrack
            selectedVideoTrack: mediaPlayer.activeVideoTrack
            selectedSubtitleTrack: mediaPlayer.activeSubtitleTrack
            visible: false
        }

        ParallelAnimation {
            id: hideControls

            NumberAnimation {
                targets: [playbackControl, seeker, background, shadow]
                property: "opacity"
                to: 0
                duration: 1000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: playbackControl
                property: "anchors.bottomMargin"
                to: -playbackControl.height - seeker.height
                duration: 1000
                easing.type: Easing.InOutQuad
            }
        }

        ParallelAnimation {
            id: showControls

            NumberAnimation {
                targets: [playbackControl, seeker, shadow]
                property: "opacity"
                to: 1
                duration: 1000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: background
                property: "opacity"
                to: 0.5
                duration: 1000
                easing.type: Easing.InOutQuad
            }
            NumberAnimation {
                target: playbackControl
                property: "anchors.bottomMargin"
                to: 0
                duration: 1000
                easing.type: Easing.InOutQuad
            }
        }

        Component.onCompleted: {
            if (source.toString().length > 0)
                openFile(source);
            else
                mediaPlayer.play();
        }
    }
}
