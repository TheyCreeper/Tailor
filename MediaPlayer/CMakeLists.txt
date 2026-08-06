qt_add_library(MediaPlayer STATIC)

set_source_files_properties(Images.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

set(
QML_IMPORT_PATH
)

set(SUBFOLDER_QML_FILES
    # Views
    ## Library
    "Views/Library/LibraryPage.qml"
    "Views/Library/AlbumView.qml"
    "Views/Library/PlaylistView.qml"
    "Views/Library/ArtistView.qml"

    ## Other
    "Views/PlayingPage.qml"
    "Views/SettingsPage.qml"

    # Components
    "Components/ErrorPopup.qml"
    "Components/MetadataInfo.qml"
    "Components/SettingsInfo.qml"
    "Components/ThemeInfo.qml"
    "Components/TouchMenu.qml"
    "Components/TracksInfo.qml"
    "Components/TracksOptions.qml"
    "Components/UrlPopup.qml"
    "Components/PlaylistInfo.qml"
    "Components/ContentPage.qml"
    "Components/DiscElement.qml"
    "Components/MetroTab.qml"
)

# Flatten their virtual paths inside the module root
foreach(qml_file IN LISTS SUBFOLDER_QML_FILES)
    get_filename_component(file_name "${qml_file}" NAME)
    set_source_files_properties("${qml_file}"
        PROPERTIES
            QT_RESOURCE_ALIAS "${file_name}"
    )
endforeach()

qt_add_qml_module(MediaPlayer
    URI "MediaPlayer"
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/MediaPlayer"
    QML_FILES
        "Main.qml"
        "Images.qml"
        ${SUBFOLDER_QML_FILES}
    RESOURCES
        "icons/Shadow.png"
        "icons/Shadow@2x.png"
        "icons/Default_CoverArt.svg"
        "icons/Movie_Active.svg"
        "icons/Movie_Icon_Dark.svg"
        "icons/Movie_Icon.svg"
        "icons/Music_Active.svg"
        "icons/Music_Icon_Dark.svg"
        "icons/Music_Icon.svg"
        "icons/Warning_Icon.svg"
    DEPENDENCIES
        TARGET Config
        TARGET MediaControls
        QML_FILES NavBar.qml
        QML_FILES Components/MetroTab.qml
        QML_FILES Views/SettingsPage.qml
)
