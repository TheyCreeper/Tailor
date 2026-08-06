import QtQuick
import QtQuick.Controls.Fusion
import QtQuick.Dialogs
import QtQuick.Layouts
import QtCore
import MediaControls

Rectangle {
    id: root
    color: "#aaabae"
    anchors.fill: root
    objectName: "navbarContainer"

    signal pageRequested(string pageUrl)

    ColumnLayout {
        NavItem {
            id: playingNavButton
            pageUrl: "PlayingPage.qml"
            text: "Playing"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
        NavItem {
            id: libraryNavButton
            pageUrl: "Views/Library/LibraryPage.qml"
            text: "Library"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
        NavItem {
            text: "Test"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
    }
}
