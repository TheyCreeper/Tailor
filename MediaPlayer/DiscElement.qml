import QtQuick
import QtQuick.Layouts
import Config

ColumnLayout {
    id: content

    property string artUrl: "Default_CoverArt"
    property string mediaNameProp: "Name"
    property string mediaSubnameProp: "Subname"

    spacing: 8

    Image {
        id: coverArt
        Layout.fillWidth: true
        Layout.preferredHeight: width
        source: Images.iconSource(content.artUrl, false)
        fillMode: Image.PreserveAspectCrop
    }

    ColumnLayout {
        id: mediaInfo
        Layout.fillWidth: true
        Layout.leftMargin: 8
        Layout.rightMargin: 8
        spacing: 2

        Text {
            id: mediaName
            text: content.mediaNameProp
            font.pixelSize: 18
            font.bold: true
            color: "white"
            Layout.fillWidth: true

            wrapMode: Text.WordWrap
            maximumLineCount: 2
            elide: Text.ElideRight
        }

        Text {
            text: mediaSubnameProp
            color: "gray"
            font.pixelSize: 16
            Layout.fillWidth: true
            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            maximumLineCount: 2
            elide: Text.ElideRight
        }
    }
}