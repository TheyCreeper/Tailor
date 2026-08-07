#include "media.h"
#include <utility>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <taglib/audioproperties.h>
using namespace std;
using namespace std::filesystem;
using namespace TagLib;
//  MediaTag Implementation

MediaTag::MediaTag(int MediaTagid, std::string name, std::string color)
    : MediaTagid_(MediaTagid), name_(std::move(name)), color_(std::move(color)) {}

//  Media Implementation

Media::Media(int itemId, std::string title, std::chrono::seconds duration,
             std::string image, std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags)
    : itemid_(itemId),
    title_(std::move(title)),
    duration_(duration),
    image_(std::move(image)),
    artist_(std::move(artist)),
    MediaTags_(std::move(MediaTags)) {}

//  Song Implementation

Song::Song(const std::string& filePath) {
    FileRef f(filePath.c_str());
    if (!f.isNull() && f.tag()) {
        const TagLib::Tag* fileTag = f.tag();

        setTitle(fileTag->title().to8Bit());
        setArtist(fileTag->artist().to8Bit());
        setAlbumName(fileTag->album().to8Bit());
        setGenre(fileTag->genre().to8Bit());
        setTrackNum(fileTag->track());
        setFilePath(filePath);
        setDuration(std::chrono::seconds(f.audioProperties()->lengthInSeconds()));
        setBitrate(f.audioProperties()->bitrate());
        setSampleRate(f.audioProperties()->sampleRate());
        setReleaseYear(fileTag->year());
        return;
    }
}

Song::Song(int itemId, std::string title, std::chrono::seconds duration,
           std::string image, std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags)
    : Media(itemId, std::move(title), duration, std::move(image), std::move(artist), std::move(MediaTags)) {}



std::chrono::seconds Song::getDuration() const {
    return getMediaDuration();
}

void Song::Play() const {
    // TODO: Audio playback implementation
}

//  Album Implementation
Album::Album(std::string title,
             std::string artist)
    : MediaCollection(std::move(title), std::move(artist)) {}

Album::Album(int itemId, std::string title, std::string image,std::string artist)
    : MediaCollection(itemId, std::move(title), std::move(image), std::move(artist)) {}

Album::Album(int itemId, std::string title, std::string image,
             std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags)
    : MediaCollection(itemId, std::move(title), std::chrono::seconds{0},
                      std::move(image), std::move(artist), std::move(MediaTags)) {}

Album::Album(int itemId, std::string title, std::string image, std::string artist,
             std::vector<std::shared_ptr<const Media>> items, std::vector<std::shared_ptr<const MediaTag>> MediaTags)
    : MediaCollection(itemId, std::move(title), std::chrono::seconds{0},
                      std::move(image), std::move(artist), std::move(MediaTags)),
    items_(std::move(items)) {}

void Album::addMedia(std::shared_ptr<const Media> item) {
    if (item) {
        items_.push_back(std::move(item));
    }
}

std::chrono::seconds Album::getDuration() const {
    std::chrono::seconds total{0};
    for (const auto& item : items_) {
        if (item) total += item->getDuration();
    }
    return total;
}

void Album::Play() const {
    // TODO: Sequentially/shuffled playback implementation
}

//  Playlist Implementation

Playlist::Playlist(int itemId, std::string title, std::string image,
                   std::string artist, std::vector<std::shared_ptr<const MediaTag>> MediaTags)
    : MediaCollection(itemId, std::move(title), std::chrono::seconds{0},
                      std::move(image), std::move(artist), std::move(MediaTags)) {}

void Playlist::addMedia(std::shared_ptr<const Media> item) {
    if (item) {
        items_.push_back(std::move(item));
    }
}

std::chrono::seconds Playlist::getDuration() const {
    std::chrono::seconds total{0};
    for (const auto& item : items_) {
        if (item) total += item->getDuration();
    }
    return total;
}

void Playlist::Play() const {
    // TODO: Playlist playback implementation
}

Artist::Artist(std::string name, std::string artPath)
    : id_(-1), name_(std::move(name)), artPath_(std::move(artPath)) {}
Artist::Artist(int id, std::string name, std::string artPath)
    : id_(id), name_(std::move(name)), artPath_(std::move(artPath)) {}