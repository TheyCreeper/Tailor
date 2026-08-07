#include "sqlhelper.h"
#include <QFileInfo>
#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

const string DB_FILE = "/home/creeper/Documents/dev/tailor/db.sql";

SqlHelper::SqlHelper() {
    db_ = QSqlDatabase::addDatabase("QSQLITE");

    dbPath_ = "/home/creeper/Documents/dev/tailor/db";

    bool dbExists = QFileInfo::exists(dbPath_);

    qDebug() << "Opening DB at absolute path:" << QFileInfo(dbPath_).absoluteFilePath();
    db_.setDatabaseName(dbPath_);

    if (!db_.open()) {
        qDebug() << "Failed to open database:" << db_.lastError().text();
        return;
    }

    QSqlQuery query(db_);
    query.exec("PRAGMA foreign_keys = ON;");

    if (!dbExists || db_.tables().isEmpty()) {
        qDebug() << "Database schema missing or new file detected. Generating DB from SQL file...";
        if (!generateDB(QString::fromStdString(DB_FILE))) {
            qDebug() << "Failed to generate database schema!";
        }
    }
}

bool SqlHelper::generateDB(const QString& sqlFilePath) {
    QFile file(sqlFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Cannot open SQL file:" << sqlFilePath << "Error:" << file.errorString();
        return false;
    }

    QTextStream in(&file);
    QString fullScript = in.readAll();
    file.close();

    QStringList rawStatements = fullScript.split(';', Qt::SkipEmptyParts);

    if (!db_.transaction()) {
        qDebug() << "Failed to start transaction for schema generation:" << db_.lastError().text();
        return false;
    }

    QSqlQuery query(db_);
    for (QString statement : rawStatements) {
        statement = statement.trimmed();

        if (statement.isEmpty() || statement.startsWith("--")) {
            continue;
        }

        if (!query.exec(statement)) {
            qDebug() << "Failed to execute SQL statement:\n" << statement;
            qDebug() << "Error:" << query.lastError().text();
            db_.rollback();
            return false;
        }
    }

    if (!db_.commit()) {
        qDebug() << "Failed to commit schema transaction:" << db_.lastError().text();
        db_.rollback();
        return false;
    }

    qDebug() << "Database schema successfully generated from" << sqlFilePath;
    return true;
}

// general functions
bool SqlHelper::PrepareSql(QSqlQuery& query, const QString& sql) {
    if (!query.prepare(sql)) {
        qDebug() << "SQL Prepare Failed:" << query.lastError().text();
        qDebug() << "Database Error Details:" << db_.lastError().text();
        if (db_.transaction()) {
            db_.rollback();
        }
        return false;
    }
    return true;
}

bool SqlHelper::ExecuteAndCommit(QSqlQuery& query) {
    if (!query.execBatch()) {
        qDebug() << "Batch insert failed:" << query.lastError().text();
        db_.rollback();
        return false;
    }

    if (!db_.commit()) {
        qDebug() << "Commit failed:" << db_.lastError().text();
        db_.rollback();
        return false;
    }
    return true;
}


// song implementation

bool SqlHelper::AddTracks(vector<shared_ptr<const Song>> songList, map<string, int>& albumIdPair) {
    if (songList.empty()) return true;

    QVariantList titles;
    QVariantList albumIds;
    QVariantList trackNumbers;
    QVariantList filePaths;
    QVariantList durations;
    QVariantList bitrates;
    QVariantList sampleRate;
    QVariantList releaseYears;

    for (const auto& song : songList) {
        if (!song) continue;

        titles << QString::fromStdString(song->getTitle());
        albumIds << albumIdPair[song->getAlbum()];
        filePaths << QString::fromStdString(song->getPath());
        trackNumbers << song->getTrackNum();
        durations << static_cast<qlonglong>(song->getDuration().count());
        bitrates << song->getBitrate();
        sampleRate << song->getSampleRate();
        releaseYears << song->getReleaseYear();
    }
    if (!db_.transaction()) {
        qDebug() << "Failed to start transaction:" << db_.lastError().text();
        return false;
    }

    QSqlQuery query(db_);
    const QString sql = "INSERT INTO \"tracks\" (filePath, title, albumId, trackNumber, durationSeconds, \"release\", sampleRate, bitrate) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";
    if (!PrepareSql(query, sql)) { return false; }
    query.addBindValue(filePaths);
    query.addBindValue(titles);
    query.addBindValue(albumIds);
    query.addBindValue(trackNumbers);
    query.addBindValue(durations);
    query.addBindValue(releaseYears);
    query.addBindValue(sampleRate);
    query.addBindValue(bitrates);
    if (!ExecuteAndCommit(query)) { return false; }
    return true;
}

vector<shared_ptr<const Song>> SqlHelper::GetTracks() {
    vector<shared_ptr<const Song>> tracks;
    QSqlQuery query(db_);
    query.prepare("SELECT id, filePath, title, albumId, trackNumber, durationSeconds, \"release\", sampleRate, bitrate FROM tracks ORDER BY id");

    if (!query.exec()) {
        qDebug() << "Failed to load tracks:" << query.lastError().text();
        return tracks;
    }

    while (query.next()) {
        const int id = query.value("id").toInt();
        const QString filePath = query.value("filePath").toString();
        const QString title = query.value("title").toString();
        const auto duration = std::chrono::seconds(query.value("durationSeconds").toInt());
        const QString artist = "";
        const int trackNumber = query.value("trackNumber").toInt();
        const int sampleRate = query.value("sampleRate").toInt();
        const int releaseYear = query.value("release").toInt();
        const int bitrate = query.value("bitrate").toInt();

        auto song = make_shared<Song>(
            id,
            title.toStdString(),
            duration,
            "",
            artist.toStdString(),
            vector<shared_ptr<const MediaTag>>()
        );
        song->setTrackNum(trackNumber);
        song->setSampleRate(sampleRate);
        song->setReleaseYear(releaseYear);
        song->setBitrate(bitrate);
        song->setFilePath(filePath.toStdString());
        tracks.push_back(song);
    }

    return tracks;
}

void SqlHelper::RemoveTrack(int id) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM tracks WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Failed to delete track:" << query.lastError().text();
    }
}

void SqlHelper::RemoveTracks(vector<int> ids) {
    if (ids.empty()) return;

    QStringList placeholders;
    for (int id : ids) {
        placeholders << "?";
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM tracks WHERE id IN (" + placeholders.join(", ") + ")");
    for (const int id : ids) {
        query.addBindValue(id);
    }

    if (!query.exec()) {
        qDebug() << "Failed to delete tracks:" << query.lastError().text();
    }
}

shared_ptr<const Song> SqlHelper::EditTrack(shared_ptr<const Song> editedSong) {
    if (!editedSong) {
        return nullptr;
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE tracks SET filePath = ?, title = ?, albumId = ?, trackNumber = ?, durationSeconds = ?, \"release\" = ?, sampleRate = ?, bitrate = ? WHERE id = ?");
    query.addBindValue(QString::fromStdString(editedSong->getPath()));
    query.addBindValue(QString::fromStdString(editedSong->getTitle()));
    query.addBindValue(1);
    query.addBindValue(editedSong->getTrackNum());
    query.addBindValue(static_cast<int>(editedSong->getDuration().count()));
    query.addBindValue(editedSong->getReleaseYear());
    query.addBindValue(editedSong->getSampleRate());
    query.addBindValue(editedSong->getBitrate());
    query.addBindValue(editedSong->getItemId());

    if (!query.exec()) {
        qDebug() << "Failed to update track:" << query.lastError().text();
        return nullptr;
    }

    return editedSong;
}

// albums
bool SqlHelper::AddAlbums(vector<shared_ptr<const Album>> AlbumList, map<string, int>& artistIdPair) {
    if (AlbumList.empty()) return true;

    QVariantList names;
    QVariantList coverPaths;
    QVariantList artistIds;
    for (const auto& album : AlbumList) {
        if (!album) continue;
        names << QString::fromStdString(album->getTitle());
        coverPaths << QString::fromStdString(album->getImage());
        artistIds << artistIdPair[album->getArtist()];
    }
    if (!db_.transaction()) {
        qDebug() << "Failed to start transaction:" << db_.lastError().text();
        return false;
    }

    QSqlQuery query(db_);
    const QString sql = "INSERT INTO \"album\" (name,coverPath,artistId)"
                        " VALUES (?,?,?);";
    if (!PrepareSql(query, sql)) { return false; }
    query.addBindValue(names);
    query.addBindValue(coverPaths);
    query.addBindValue(artistIds);
    if (!ExecuteAndCommit(query)) { return false; }

    return true;
}
vector<shared_ptr<const Album>> SqlHelper::GetAlbums() {
    vector<shared_ptr<const Album>> albums;
    QSqlQuery query(db_);
    query.prepare("SELECT id, name, coverPath, artistId FROM album ORDER BY id");

    if (!query.exec()) {
        qDebug() << "Failed to load albums:" << query.lastError().text();
        return albums;
    }

    while (query.next()) {
        const int id = query.value("id").toInt();
        const QString title = query.value("name").toString();
        const QString coverPath = query.value("coverPath").toString();
        const QString artist = query.value("artistId").toString();
        auto album = make_shared<Album>(
            id,
            title.toStdString(),
            coverPath.toStdString(),
            artist.toStdString()
        );
        albums.push_back(album);
    }

    return albums;
}
bool SqlHelper::RemoveAlbum(int id) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM album WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Failed to delete album:" << query.lastError().text();
        return false;
    }
    return true;
}
bool SqlHelper::RemoveAlbums(vector<int> ids) {
    if (ids.empty()) return true;

    QStringList placeholders;
    for (int id : ids) {
        placeholders << "?";
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM album WHERE id IN (" + placeholders.join(", ") + ")");
    for (const int id : ids) {
        query.addBindValue(id);
    }

    if (!query.exec()) {
        qDebug() << "Failed to delete albums:" << query.lastError().text();
        return false;
    }
    return true;
}
shared_ptr<const Album> SqlHelper::EditAlbum(shared_ptr<const Album> editedAlbum) {
    if (!editedAlbum) {
        return nullptr;
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE album SET name = ?, coverPath = ?, artistId = ? WHERE id = ?");
    query.addBindValue(QString::fromStdString(editedAlbum->getTitle()));
    query.addBindValue(QString::fromStdString(editedAlbum->getImage()));
    query.addBindValue(1);
    query.addBindValue(editedAlbum->getItemId());

    if (!query.exec()) {
        qDebug() << "Failed to update album:" << query.lastError().text();
        return nullptr;
    }

    return editedAlbum;
}

// artists
bool SqlHelper::AddArtists(vector<shared_ptr<const Artist>> ArtistList) {
    if (ArtistList.empty()) return true;

    QVariantList names;
    QVariantList coverPaths;

    for (const auto& artist : ArtistList) {
        if (!artist) continue;
        names << QString::fromStdString(artist->getName());
        coverPaths << QString::fromStdString(artist->getArtPath());
    }
    if (!db_.transaction()) {
        qDebug() << "Failed to start transaction:" << db_.lastError().text();
        return false;
    }

    QSqlQuery query(db_);
    const QString sql = "INSERT INTO \"artist\" (\"name\",\"artPath\") "
                        "VALUES (?, ?)";
    if (!PrepareSql(query, sql)) { return false; }
    query.addBindValue(names);
    query.addBindValue(coverPaths);
    if (!ExecuteAndCommit(query)) { return false; }

    return true;

}
vector<shared_ptr<const Artist>> SqlHelper::GetArtists() {
    vector<shared_ptr<const Artist>> artists;
    QSqlQuery query(db_);
    query.prepare("SELECT id, name, artPath FROM artist ORDER BY id");

    if (!query.exec()) {
        qDebug() << "Failed to load artists:" << query.lastError().text();
        return artists;
    }

    while (query.next()) {
        const int id = query.value("id").toInt();
        const QString name = query.value("name").toString();
        const QString artPath = query.value("artPath").toString();
        artists.push_back(make_shared<Artist>(id, name.toStdString(), artPath.toStdString()));
    }

    return artists;
}
bool SqlHelper::RemoveArtist(int id) {
    QSqlQuery query(db_);
    query.prepare("DELETE FROM artist WHERE id = ?");
    query.addBindValue(id);
    if (!query.exec()) {
        qDebug() << "Failed to delete artist:" << query.lastError().text();
        return false;
    }
    return true;
}
bool SqlHelper::RemoveArtists(vector<int> ids) {
    if (ids.empty()) return true;

    QStringList placeholders;
    for (int id : ids) {
        placeholders << "?";
    }

    QSqlQuery query(db_);
    query.prepare("DELETE FROM artist WHERE id IN (" + placeholders.join(", ") + ")");
    for (const int id : ids) {
        query.addBindValue(id);
    }

    if (!query.exec()) {
        qDebug() << "Failed to delete artists:" << query.lastError().text();
        return false;
    }
    return true;
}
shared_ptr<const Artist> SqlHelper::EditArtist(shared_ptr<const Artist> editedArtist) {
    if (!editedArtist) {
        return nullptr;
    }

    QSqlQuery query(db_);
    query.prepare("UPDATE artist SET name = ?, artPath = ? WHERE id = ?");
    query.addBindValue(QString::fromStdString(editedArtist->getName()));
    query.addBindValue(QString::fromStdString(editedArtist->getArtPath()));
    query.addBindValue(editedArtist->getId());

    if (!query.exec()) {
        qDebug() << "Failed to update artist:" << query.lastError().text();
        return nullptr;
    }

    return editedArtist;
}

