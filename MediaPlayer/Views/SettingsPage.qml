import QtQuick
import QtQuick.Layouts
import QtQuick.Controls.Fusion

ContentPage {
    id: root
    title: "Settings"

    ColumnLayout {
        id: content

        TextField {
            id: inputField
            // Direct binding to C++ NOTIFY property (No Connections block needed)
            text: LibraryBackend.statusMessage
        }

        Button {
            text: "Test"
            onClicked: {
                LibraryBackend.scanLibrary(inputField.text)
            }
        }
    }
}