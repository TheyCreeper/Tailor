import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent

    property string title: "Page Title"
    Text {
        id: pageTitle
        text: title
        font.pixelSize: 40
        anchors.left: parent.left
        topPadding: 36
        leftPadding: 36
        color: "white"
    }
    default property alias content: item.children
    Item {
        id: item
        anchors.fill: parent
    }
}
