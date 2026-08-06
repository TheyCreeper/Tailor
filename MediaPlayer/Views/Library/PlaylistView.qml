import QtQuick
import MediaControls
import Config

GridView {
    id: gridView
    anchors.fill: parent
    clip: true

    property real minCellWidth: 180
    property int columns: Math.max(1, Math.floor(width / minCellWidth))

    cellWidth: width / columns
    cellHeight: cellWidth + 70
    model: 12
    delegate: Item {
        width: gridView.cellWidth
        height: gridView.cellHeight
        DiscElement {
            anchors.fill: parent
            anchors.margins: 8
            artUrl: "Default_CoverArt"
            mediaNameProp: "Album Title " + (index + 1)
            mediaSubnameProp: "Artist / Subtitle info goes here"
        }
    }
}