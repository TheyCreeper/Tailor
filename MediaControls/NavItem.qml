import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Layouts
import Config
Button {
    id: content
    signal navTriggered(string targetPage)
    property string pageUrl: ""
    Layout.fillWidth: true
    Layout.leftMargin: 8
    Layout.rightMargin: 8
    Layout.topMargin: 4
    Layout.bottomMargin: 4
    onClicked: {
        onClicked: {
                if (pageUrl !== "") {
                    navTriggered(pageUrl)
                }
            }
    }

}
