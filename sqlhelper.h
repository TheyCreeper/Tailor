#ifndef SQLHELPER_H
#define SQLHELPER_H
#include <media.h>
#include <QtSql>
#include <QSqlDatabase>
#include <map>

using namespace std;

class SqlHelper
{
private:
    QSqlDatabase db_;
public:
    SqlHelper();
    virtual ~SqlHelper() = default;

    // tracks
    bool AddTracks(vector<shared_ptr<const Song>> songList/*, map<string, int> albumIdPair*/);
    vector<shared_ptr<const Song>> GetTracks();
    void RemoveTrack(int id);
    void RemoveTracks(vector<int> id);
    shared_ptr<const Song> EditTrack(shared_ptr<const Song> editedSong);

    // albums
    bool AddAlbums(vector<shared_ptr<const Album>> AlbumList, map<string, int> artistIdPair);
    vector<shared_ptr<const Album>> GetAlbums();
    bool RemoveAlbum(int id);
    bool RemoveAlbums(vector<int> id);
    shared_ptr<const Album> EditAlbum(shared_ptr<const Album> editedAlbum);

    // artists
    bool AddArtists(vector<shared_ptr<const Artist>> ArtistList);
    vector<shared_ptr<const Artist>> GetArtists();
    bool RemoveArtist(int id);
    bool RemoveArtists(vector<int> id);
    shared_ptr<const Artist> EditArtist(shared_ptr<const Artist> editedArtist);

    QSqlDatabase& getDb() { return db_; }
};

#endif // SqlHelper_H
