// mediacontroller.cpp
#include "libraryhelper.h"
#include "media.h"
#include <QDebug>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
using namespace std;
using namespace std::filesystem;
using namespace TagLib;

LibraryHelper::LibraryHelper(QObject *parent) : QObject(parent) {}

void LibraryHelper::scanLibrary(const QString &folderPath) {
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    // db.setDatabaseName("./db.db");

    auto songList = scanSongs(folderPath.toStdString());
    auto albumList = scanAlbums(songList);

    qDebug() << "Scanning folder:" << folderPath;
    emit statusMessageChanged("Started scanning: " + folderPath);
}


std::vector<std::shared_ptr<const Song>> LibraryHelper::scanSongs(const std::string &libraryPath) {
    std::vector<std::shared_ptr<const Song>> list;
    std::error_code ec;

    auto iter = std::filesystem::recursive_directory_iterator(
        libraryPath,
        std::filesystem::directory_options::skip_permission_denied,
        ec
        );

    if (ec) {
        qWarning() << "Failed to open directory:" << QString::fromStdString(ec.message());
        return list;
    }

    for (const auto& entry : iter) {
        if (entry.is_regular_file(ec)) {
            auto ext = entry.path().extension().string();
            if (ext == ".mp3" || ext == ".flac" || ext == ".m4a" || ext == ".wav") {
                list.push_back(std::make_shared<const Song>(entry.path().string()));
            }
        }
    }

    return list;
}

std::vector<std::shared_ptr<const Album>> LibraryHelper::scanAlbums(std::vector<std::shared_ptr<const Song>> songList) {
    std::vector<std::shared_ptr<const Album>> list;
    std::error_code ec;
    string tempName = "";

    while (songList.size() != 0) {
        tempName = songList[0]->getAlbum();
        list.push_back(std::make_shared<const Album>(
            songList[0]->getAlbum(),
            songList[0]->getArtist()));

        std::erase_if(songList, [&tempName](const std::shared_ptr<const Song>& item) {
            return item->getAlbum() == tempName;
        });
    }

    return list;
}




