import QtQuick
import QtQuick.Layouts
import Config

RowLayout {
    id: content
    property string artUrl: "Default_CoverArt"
    property string mediaNameProp: "Name"
    property string mediaSubnameProp: "Subname"

    Image {
        id: coverArt
        anchors.verticalCenter: parent.verticalCenter
        width: Math.min(parent.width, parent.height) * 0.6
        height: width
        source: Images.iconSource(artUrl, false)
        fillMode: Image.Stretch
    }
    Rectangle {
        id: mediaInfo
        anchors.left: coverArt
        color: "transparent"
        anchors.bottom: coverArt.bottom
        anchors.horizontalCenter: coverArt.right
        anchors.leftMargin: 36
        Text {
            id: mediaName
            text: mediaNamePop
            font.pixelSize: 32
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            color: "white"
        }
        Text {
            id: mediaSubName
            text: mediaSubnameProp
            font.pixelSize: 16
            anchors.top: songName.bottom
            anchors.horizontalCenter: songName.left
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            topPadding: 12
            color: "grey"
        }
    }
}
