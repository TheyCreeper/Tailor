#include "sqlhelper.h"

SqlHelper::SqlHelper() {
    db_ = QSqlDatabase::addDatabase("QSQLITE");

    db_.setDatabaseName("./db.db");

    // publish library to the database
    if (db_.open()) {
        return;
    }
}

// song implementation

bool SqlHelper::AddTracks(vector<shared_ptr<const Song>> songList/*, map<string, int> albumIdPair*/) {
    QVariantList titles;
    QVariantList albumIds;
    QVariantList genres;
    QVariantList trackNumbers;
    QVariantList filePaths;
    QVariantList durations;
    QVariantList bitrates;
    QVariantList sampleRate;
    QVariantList releaseYears;

    for (const auto& song : songList) {
        titles << QString::fromStdString(song->getTitle());
        genres << QString::fromStdString(song->getGenre());
        // albumIds << albumIdPair[song->getAlbum()];
        albumIds << 1;
        filePaths << QString::fromStdString(song->getPath());
        trackNumbers << song->getTrackNum();
        durations.append(QVariant::fromValue(song->getDuration().count()));
        bitrates << song->getBitrate();
        sampleRate << song->getSampleRate();
        releaseYears << song->getReleaseYear();
    }

    // insert into the db

    QSqlQuery query(db_);
    query.prepare("INSERT INTO \"tracks\" (\"filePath\", \"title\", \"albumId\", \"trackNumber\", \"durationSeconds\", \"release\", \"sampleRate\", \"bitrate\") "
                  "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
    query.addBindValue(filePaths);
    query.addBindValue(titles);
    query.addBindValue(albumIds);
    query.addBindValue(trackNumbers);
    query.addBindValue(durations);
    query.addBindValue(releaseYears);
    query.addBindValue(sampleRate);
    query.addBindValue(bitrates);

    if (!query.execBatch()) {
        qDebug() << "Batch insert failed:" << query.lastError().text();
        db_.rollback();
        return false;
    }

    // 5. Commit the transaction to disk
    if (!db_.commit()) {
        qDebug() << "Commit failed:" << db_.lastError().text();
        db_.rollback();
        return false;
    }
    return true;
}

vector<shared_ptr<const Song>> SqlHelper::GetTracks() {
}

void SqlHelper::RemoveTrack(int id) {

}

void SqlHelper::RemoveTracks(vector<int> id) {

}

shared_ptr<const Song> SqlHelper::EditTrack(shared_ptr<const Song> editedSong) {
}


