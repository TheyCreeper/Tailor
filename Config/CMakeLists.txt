qt_add_library(Config STATIC)

set_source_files_properties(Config.qml
    PROPERTIES
        QT_QML_SINGLETON_TYPE true
)

qt_add_qml_module(Config
    URI "Config"
    OUTPUT_DIRECTORY "${QT_QML_OUTPUT_DIRECTORY}/Config"
    QML_FILES
        "Config.qml"
)
