import QtQuick
import QtQuick.Layouts
import Config

RowLayout {
    id: content
    property string artUrl: "Default_CoverArt"
    property string mediaNameProp: "Name"
    property string mediaSubnameProp: ""

    Image {
        id: coverArt
        anchors.horizontalCenter: contentPage.left
        anchors.verticalCenter: parent.verticalCenter
        width: Math.min(parent.width, parent.height) * 0.6
        height: width
        source: Images.iconSource(artUrl, false)
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
            text: songNameProp
            font.pixelSize: 32
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            color: "white"
        }
        Text {
            id: mediaSubName
            text: mediaSubNameProp
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
