// mediacontroller.cpp
#include "libraryhelper.h"
#include "media.h"
#include <QDebug>
#include <filesystem>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
#include <sqlhelper.h>
using namespace std;
using namespace filesystem;
using namespace TagLib;

LibraryHelper::LibraryHelper(QObject *parent) : QObject(parent) {}

void LibraryHelper::scanLibrary(const QString &folderPath) {
    SqlHelper sqlHelper;
    auto songList = scanSongs(folderPath.toStdString());
    auto albumList = scanAlbums(songList);
    auto artistList = scanArtist(albumList);

    if (!artistList.empty()) {
        sqlHelper.AddArtists(artistList);
    }

    std::map<std::string, int> artistIdByName;
    for (const auto& artist : sqlHelper.GetArtists()) {
        if (artist) {
            artistIdByName[artist->getName()] = artist->getId();
        }
    }

    if (!albumList.empty()) {
        sqlHelper.AddAlbums(albumList, artistIdByName);
    }

    std::map<std::string, int> albumIdByName;
    for (const auto& album : sqlHelper.GetAlbums()) {
        if (album) {
            albumIdByName[album->getTitle()] = album->getItemId();
        }
    }

    if (!songList.empty()) {
        sqlHelper.AddTracks(songList, albumIdByName);
    }

    qDebug() << "Scanning folder:" << folderPath;
    emit statusMessageChanged("Started scanning: " + folderPath);
}

vector<shared_ptr<const Song>> LibraryHelper::scanSongs(const string &libraryPath) {
    vector<shared_ptr<const Song>> list;
    error_code ec;

    auto iter = filesystem::recursive_directory_iterator(
        libraryPath,
        filesystem::directory_options::skip_permission_denied,
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
                list.push_back(make_shared<const Song>(entry.path().string()));
            }
        }
    }

    return list;
}

vector<shared_ptr<const Album>> LibraryHelper::scanAlbums(vector<shared_ptr<const Song>> songList) {
    vector<shared_ptr<const Album>> list;
    string tempName = "";

    while (songList.size() != 0) {
        tempName = songList[0]->getAlbum();
        list.push_back(make_shared<const Album>(
            songList[0]->getAlbum(),
            songList[0]->getArtist()));

        erase_if(songList, [&tempName](const shared_ptr<const Song>& item) {
            return item->getAlbum() == tempName;
        });
    }
    return list;
}

vector<shared_ptr<const Artist>> LibraryHelper::scanArtist(vector<shared_ptr<const Album>> albumList) {
    vector<shared_ptr<const Artist>> list;
    string tempName = "";

    while (albumList.size() != 0) {
        tempName = albumList[0]->getArtist();
        list.push_back(make_shared<const Artist>(
            albumList[0]->getArtist()));

        erase_if(albumList, [&tempName](const shared_ptr<const Album>& item) {
            return item->getArtist() == tempName;
        });
    }
    return list;
}




