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

    // It would be nice to be able to make this abstract and be able to declare it
    // in the main in the future
    ColumnLayout {
        NavItem {
            id: playingNavButton
            pageUrl: "PlayingPage.qml"
            text: "Playing"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
        NavItem {
            id: libraryNavButton
            pageUrl: "LibraryPage.qml"
            text: "Library"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
        NavItem {
            text: "Settings"
            pageUrl: "SettingsPage.qml"
            onNavTriggered: (url) => navbar.pageRequested(url)
        }
    }
}
