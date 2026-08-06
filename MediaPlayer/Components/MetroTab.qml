// Components/MetroTab.qml
import QtQuick
import QtQuick.Controls

Item {
    id: root
    implicitWidth: tabBar.contentWidth
    implicitHeight: 48

    property alias currentIndex: tabBar.currentIndex
    property var tabsModel: [] // Model array: [{ name: "TRACKS", page: "TracksInfo.qml" }]
    property color accentColor: "#0078D7"
    property color activeTextColor: "#FFFFFF"
    property color inactiveTextColor: "#888888"

    // Signal emitted when a tab is clicked
    signal tabClicked(int index, var page)

    TabBar {
        id: tabBar
        anchors.fill: parent
        spacing: 24
        background: Rectangle { color: "transparent" }

        Repeater {
            model: root.tabsModel

            TabButton {
                id: button
                // Extract 'name' property from JS object or fall back to standard string
                text: (modelData && modelData.name !== undefined) ? modelData.name : modelData
                implicitWidth: textItem.implicitWidth + 12
                implicitHeight: root.height

                onClicked: {
                    var targetPage = (modelData && modelData.page !== undefined) ? modelData.page : null
                    root.tabClicked(index, targetPage)
                }

                contentItem: Text {
                    id: textItem
                    text: button.text
                    font.pixelSize: 15
                    font.weight: button.checked ? Font.DemiBold : Font.Normal
                    font.capitalization: Font.AllUppercase
                    color: button.checked ? root.activeTextColor : root.inactiveTextColor
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Behavior on color { ColorAnimation { duration: 150 } }
                }

                background: Rectangle {
                    color: button.hovered && !button.checked ? "#10FFFFFF" : "transparent"
                }
            }
        }
    }

    // Animated Accent Line
    Rectangle {
        id: indicator
        height: 3
        color: root.accentColor
        anchors.bottom: parent.bottom

        readonly property Item currentItem: tabBar.currentItem
        x: currentItem ? currentItem.x : 0
        width: currentItem ? currentItem.width : 0

        Behavior on x { NumberAnimation { duration: 250; easing.type: Easing.OutCubic } }
        Behavior on width { NumberAnimation { duration: 250; easing.type: Easing.OutCubic } }
    }
}