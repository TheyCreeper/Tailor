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

// song implementation

bool SqlHelper::AddTracks(vector<shared_ptr<const Song>> songList/*, map<string, int> albumIdPair*/) {
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
        albumIds << 1;
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

    // Note: Escaped "release" because it is an SQLite reserved keyword!
    const QString sql = "INSERT INTO \"tracks\" (filePath, title, albumId, trackNumber, durationSeconds, \"release\", sampleRate, bitrate) "
                        "VALUES (?, ?, ?, ?, ?, ?, ?, ?)";

    // 1. MUST check if prepare succeeds
    if (!query.prepare(sql)) {
        qDebug() << "SQL Prepare Failed:" << query.lastError().text();
        qDebug() << "Database Error Details:" << db_.lastError().text();
        db_.rollback();
        return false;
    }

    // 2. Add lists in exact order of the columns above
    query.addBindValue(filePaths);
    query.addBindValue(titles);
    query.addBindValue(albumIds);
    query.addBindValue(trackNumbers);
    query.addBindValue(durations);
    query.addBindValue(releaseYears);
    query.addBindValue(sampleRate);
    query.addBindValue(bitrates);

    // 3. Execute batch
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

vector<shared_ptr<const Song>> SqlHelper::GetTracks() {
}

void SqlHelper::RemoveTrack(int id) {

}

void SqlHelper::RemoveTracks(vector<int> id) {

}

shared_ptr<const Song> SqlHelper::EditTrack(shared_ptr<const Song> editedSong) {
}


