import QtQuick
import MediaControls
import Config
import QtQuick.Layouts
import QtQuick.Controls
import MediaPlayer

ContentPage {
    id: root
    title: "Library"
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        MetroTab {
            id: metroTabs
            Layout.fillWidth: false

            // Define names and target pages
            tabsModel: [
                {
                    name: "ALBUM",
                    page: "AlbumView.qml"
                },
                {
                    name: "ARTIST",
                    page: "ArtistView.qml"
                },
                {
                    name: "PLAYLIST",
                    page: "PlaylistView.qml"
                }
            ]

            // On tab click, replace the StackView item
            onTabClicked: (index, page) => {
                if (page) {
                    stackView.replace(page, StackView.Immediate);
                }
            }
        }

        // Need to change the
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true

            // Set initial page from the first tab model entry
            initialItem: metroTabs.tabsModel[0].page
        }
    }
}
