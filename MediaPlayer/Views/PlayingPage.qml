import QtQuick
import QtQuick.Controls.Fusion

ContentPage {
    id: contentPage
    title: "Now Playing"

    property string albumArtUrl: "Default_CoverArt"
    property string songNameProp: "song name"
    property string songAlbumProp: "song album"
    property string songArtistProp: "song artist"
    property string songYearProp: "song year"

    Image {
        id: coverArt
        anchors.horizontalCenter: contentPage.left
        anchors.verticalCenter: parent.verticalCenter
        width: Math.min(parent.width, parent.height) * 0.6
        height: width
        source: Images.iconSource(albumArtUrl, false)
    }

    Rectangle {
        id: songInfo
        anchors.left: coverArt
        color: "transparent"
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: coverArt.right
        anchors.leftMargin: 36

        Text {
            id: songName
            text: songNameProp
            font.pixelSize: 32
            color: "white"
        }
        Text {
            id: songAlbum
            text: songAlbumProp
            font.pixelSize: 16
            anchors.top: songName.bottom
            topPadding: 12
            color: "grey"
        }

        Text {
            id: splitter
            text: "•"
            font.pixelSize: 16
            color: "grey"
            anchors.top: songName.bottom
            anchors.left: songAlbum.right
            leftPadding: 4
            topPadding: 12
        }

        Text {
            id: songArtist
            text: songArtistProp
            font.pixelSize: 16
            color: "grey"
            anchors.top: songName.bottom
            anchors.left: splitter.right
            leftPadding: 4
            topPadding: 12
        }
    }

    Rectangle {
        id: background
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: seeker.opacity ? seeker.top : playbackControl.top
        color: Config.mainColor
        opacity: videoOutput.fullScreen ? 0.75 : 0.5
    }
}
