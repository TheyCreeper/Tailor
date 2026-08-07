// mediacontroller.h
#pragma once
#include <QObject>
#include <QString>
#include <QtSql>
#include <QSqlDatabase>
#include "media.h"

class LibraryHelper : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool isScanning READ isScanning NOTIFY isScanningChanged)

public:
    explicit LibraryHelper(QObject *parent = nullptr);

    bool isScanning() const { return m_isScanning; }

    Q_INVOKABLE void scanLibrary(const QString &folderPath);

signals:
    void isScanningChanged();
    void statusMessageChanged(const QString &message);

private:
    bool m_isScanning{false};
    std::vector<std::shared_ptr<const Song>> scanSongs(const std::string &path);
    std::vector<std::shared_ptr<const Album>> scanAlbums(std::vector<std::shared_ptr<const Song>> songList);
    std::vector<std::shared_ptr<const Artist>> scanArtist(std::vector<std::shared_ptr<const Album>> albumList);
};