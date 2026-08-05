import QtQuick
import MediaControls
import Config
ContentPage {
    id: root
    title: "Library"
    // TODO: Have a zone for the filters on the right or left or smt
    // Tabbed interface like the metro tabs for playlist, albums, artists, etc.
    // maybe have a tab for like custom moods or smt
    Grid {
        anchors.fill: parent
        flow: Grid.LeftToRight
        DiscElement {
            artUrl: "Default_CoverArt"
            mediaNameProp: "testMedia"
            mediaSubnameProp: "test media undername lalalalalalalaal"
        }
        DiscElement {
            artUrl: "Default_CoverArt"
            mediaNameProp: "testMedia"
            mediaSubnameProp: "test media undername lalalalalalalaal"
        }
        DiscElement {
            artUrl: "Default_CoverArt"
            mediaNameProp: "testMedia"
            mediaSubnameProp: "test media undername lalalalalalalaal"
        }
    }

}
