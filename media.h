#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <memory>
using namespace std;

class MediaTag {
private:
    int MediaTagid_{0};
    string name_;
    string color_;

public:
    MediaTag(int MediaTagid, string name, string color);

    int getMediaTagId() const { return MediaTagid_; }
    const string& getName() const { return name_; }
    const string& getColor() const { return color_; }
};

class Media {
private:
    int itemid_{0};
    string title_;
    chrono::seconds duration_{0};
    string image_;
    string artist_;
    vector<shared_ptr<const MediaTag>> MediaTags_;

protected:
    // Protected setters accessible only to Media and derived classes (Song, Album, Playlist)
    void setItemId(int itemId) { itemid_ = itemId; }
    void setTitle(string title) { title_ = std::move(title); }
    void setDuration(chrono::seconds duration) { duration_ = duration; }
    void setImage(string image) { image_ = std::move(image); }
    void setArtist(string artist) { artist_ = std::move(artist); }
    void setMediaTags(vector<shared_ptr<const MediaTag>> tags) { MediaTags_ = std::move(tags); }
    chrono::seconds getMediaDuration() const { return duration_; }

public:
    Media() = default;
    Media(string title, string artist)
        : title_(std::move(title)), artist_(std::move(artist)) {}
    Media(int id, string name, string coverPath, string artistId) {}
    Media(int itemId, string title, chrono::seconds duration,
          string image, string artist, vector<shared_ptr<const MediaTag>> MediaTags);

    virtual ~Media() = default;

    int getItemId() const { return itemid_; }
    const string& getTitle() const { return title_; }
    const string& getImage() const { return image_; }
    const string& getArtist() const { return artist_; }

    virtual chrono::seconds getDuration() const = 0;
    virtual void Play() const = 0;
};

class Song : public Media {
    friend class SqlHelper;
private:
    string albumName_ ;
    string genre_ ;
    string filePath_;
    int trackNum_ ;
    int bitrate_;
    int sampleRate_;
    int releaseYear_;
protected:
    void setAlbumName(string albumName) { albumName_ = std::move(albumName); }
    void setGenre(string genre) { genre_ = std::move(genre); }
    void setFilePath(string filePath) { filePath_ = filePath; }
    void setTrackNum(int trackNum) { trackNum_ = trackNum; }
    void setBitrate(int bitrate) { bitrate_ = bitrate; }
    void setSampleRate(int sampleRate) { sampleRate_ = sampleRate; }
    void setReleaseYear(int releaseYear) { releaseYear_ = releaseYear; }
public:
    explicit Song(const string& filePath);
    Song(int itemId, string title, chrono::seconds duration,
         string image, string artist, vector<shared_ptr<const MediaTag>> MediaTags);
    Song(string title, chrono::seconds duration, string artist);

    const string& getAlbum() const { return albumName_; }
    const string& getGenre() const { return genre_; }
    const string& getPath() const { return filePath_; }
    const int getTrackNum() const { return trackNum_; }
    const int getBitrate() const { return bitrate_; }
    const int getSampleRate() const { return sampleRate_; }
    const int getReleaseYear() const { return releaseYear_; }

    chrono::seconds getDuration() const override;
    void Play() const override;
};

class MediaCollection : public Media {
public:
    using Media::Media;
    virtual ~MediaCollection() = default;
    virtual void addMedia(shared_ptr<const Media> item) = 0;
    virtual size_t getItemCount() const = 0;
};

class Album : public MediaCollection {
private:
    vector<shared_ptr<const Media>> items_;
    bool isShuffle_{false};

public:
    Album(string title, string artist);
    Album(int itemId, string title, string image, string artist);
    Album(int itemId, string title, string image,
          string artist, vector<shared_ptr<const MediaTag>> MediaTags);

    Album(int itemId, string title, string image, string artist,
          vector<shared_ptr<const Media>> items, vector<shared_ptr<const MediaTag>> MediaTags);

    void addMedia(shared_ptr<const Media> item) override;
    size_t getItemCount() const override { return items_.size(); }
    chrono::seconds getDuration() const override;
    void Play() const override;
};

class Playlist : public MediaCollection {
private:
    string ownerUsername_;
    vector<shared_ptr<const Media>> items_;

public:
    Playlist(int itemId, string title, string image,
             string artist, vector<shared_ptr<const MediaTag>> MediaTags);

    void addMedia(shared_ptr<const Media> item) override;
    size_t getItemCount() const override { return items_.size(); }
    chrono::seconds getDuration() const override;
    void Play() const override;
};

class Artist {
    friend class SqlHelper;
private:
    int id_;
    string name_;
    string artPath_;
protected:
    void setId(int id) { id_ = id; }
    void setName(string name) { name_ = name; }
    void setArtPath(string artPath) { artPath_ = artPath; }
public:
    Artist(string name, string artPath = "");
    Artist(int id, string name, string artPath);

    int getId() const {return id_;}
    const string& getName() const { return name_; }
    const string& getArtPath() const { return artPath_; }
};
