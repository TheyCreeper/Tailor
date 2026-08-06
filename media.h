#pragma once

#include <string>
#include <chrono>
#include <vector>
#include <memory>

class MediaTag {
private:
    int MediaTagid_{0};
    std::string name_;
    std::string color_;

public:
    MediaTag(int MediaTagid, std::string name, std::string color);

    int getMediaTagId() const { return MediaTagid_; }
    const std::string& getName() const { return name_; }
    const std::string& getColor() const { return color_; }
};

class Media {
private:
    int itemid_{0};
    std::string title_;
    std::chrono::seconds duration_{0};
    std::string image_;
    std::string artist_;
    std::vector<std::shared_ptr<const MediaTag>> MediaTags_;

protected:
    // Protected setters accessible only to Media and derived classes (Song, Album, Playlist)
    void setItemId(int itemId) { itemid_ = itemId; }
    void setTitle(std::string title) { title_ = std::move(title); }
    void setDuration(std::chrono::seconds duration) { duration_ = duration; }
    void setImage(std::string image) { image_ = std::move(image); }
    void setArtist(std::string artist) { artist_ = std::move(artist); }
    void setMediaTags(std::vector<std::shared_ptr<const MediaTag>> tags) { MediaTags_ = std::move(tags); }
    std::chrono::seconds getMediaDuration() const { return duration_; }

public:
    Media() = default;
    Media(std::string title, std::string artist)
        : title_(std::move(title)), artist_(std::move(artist)) {}
    Media(int itemId, std::string title, std::chrono::seconds duration,
          std::string image, std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags);

    virtual ~Media() = default;

    int getItemId() const { return itemid_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getImage() const { return image_; }
    const std::string& getArtist() const { return artist_; }

    virtual std::chrono::seconds getDuration() const = 0;
    virtual void Play() const = 0;
};

class Song : public Media {
private:
    std::string albumName_ ;
    std::string genre_ ;
    int trackNum_ ;
protected:
    void setAlbumName(std::string albumName) { albumName_ = std::move(albumName); }
    void setGenre(std::string genre) { genre_ = std::move(genre); }
    void setTrackNum(int trackNum) { trackNum_ = trackNum; }
public:
    explicit Song(const std::string& filePath);
    Song(int itemId, std::string title, std::chrono::seconds duration,
         std::string image, std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags);
    Song(std::string title, std::chrono::seconds duration, std::string artist);

    const std::string& getAlbum() const { return albumName_; }
    const std::string& getGenre() const { return genre_; }

    std::chrono::seconds getDuration() const override;
    void Play() const override;
};

class MediaCollection : public Media {
public:
    using Media::Media;
    virtual ~MediaCollection() = default;
    virtual void addMedia(std::shared_ptr<const Media> item) = 0;
    virtual size_t getItemCount() const = 0;
};

class Album : public MediaCollection {
private:
    std::vector<std::shared_ptr<const Media>> items_;
    bool isShuffle_{false};

public:
    Album(std::string title, std::string artist);
    Album(int itemId, std::string title, std::string image,
          std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags);

    Album(int itemId, std::string title, std::string image, std::string artist,
          std::vector<std::shared_ptr<const Media>> items, std::vector<std::shared_ptr<const MediaTag>> MediaTags);

    void addMedia(std::shared_ptr<const Media> item) override;
    size_t getItemCount() const override { return items_.size(); }
    std::chrono::seconds getDuration() const override;
    void Play() const override;
};

class Playlist : public MediaCollection {
private:
    std::string ownerUsername_;
    std::vector<std::shared_ptr<const Media>> items_;

public:
    Playlist(int itemId, std::string title, std::string image,
             std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags);

    void addMedia(std::shared_ptr<const Media> item) override;
    size_t getItemCount() const override { return items_.size(); }
    std::chrono::seconds getDuration() const override;
    void Play() const override;
};