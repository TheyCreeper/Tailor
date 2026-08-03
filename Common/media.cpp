#include <string>
#include <chrono>
#include <vector>
#include <memory>

class Tag{
private:
    int tagid_;
    std::string name_;
    std::string color_;
public:
    Tag(int tagid, std::string name, std::string color)
        : tagid_(tagid), name_(std::move(name)), color_(std::move(color)) {}

    const int getTagId() const {return tagid_;}
    const std::string& getName() const {return name_;}
    const std::string& getColor() const {return color_;}
};

class Media {
private:
    int itemid_{0};
    std::string title_;
    std::chrono::seconds duration_{0};
    std::string image_;
    std::string artist_;
    std::vector<std::shared_ptr<const Tag>> tags_;
public:
    Media() = default;

    Media(int itemId, std::string title, std::chrono::seconds duration, std::string image, std::string artist, std::vector<std::shared_ptr<const Tag>> tags)
        : itemid_(itemId), title_(std::move(title)), duration_(duration), image_(std::move(image)), artist_(std::move(artist)), tags_(std::move(tags)) {}

    virtual ~Media() = default;

    int getItemId() const { return itemid_; }
    const std::string& getTitle() const { return title_; }
    const std::string& getImage() const { return image_; }
    const std::string& getArtist() const { return artist_; }

    virtual std::chrono::seconds getDuration() const = 0;
    virtual void Play() const = 0;
};

class Song: public Media {
private:
public:
    Song(std::string& filePath) {
        // generate the things from the metadata,
        // if impossible name it the file
    }

    Song(int itemId, std::string title, std::chrono::seconds duration, std::string image, std::string artist, std::vector<std::shared_ptr<const Tag>> tags)
        : Media(itemId, std::move(title), duration, std::move(image), std::move(artist), std::move(tags)) {}

    void Play() const override {
        // TODO
    }
};

class MediaCollection : public Media {
public:
    using Media::Media;

    virtual ~MediaCollection() = default;
    virtual void addMedia(std::shared_ptr<const Media> item) = 0;
    virtual size_t getItemCount() const = 0;
};

class Album: public MediaCollection {
private:
    std::vector<std::shared_ptr<const Media>> items_;
    bool isShuffle = false;
public:
    Album(int itemId, std::string title, std::string image, std::string artist, std::vector<std::shared_ptr<const Tag>> tags)
        : MediaCollection(itemId, std::move(title), std::chrono::seconds{0}, std::move(image), std::move(artist), std::move(tags)) {}

    Album(int itemId, std::string title, std::string image, std::string artist, std::vector<std::shared_ptr<const Media>> items,  std::vector<std::shared_ptr<const Tag>> tags)
        : MediaCollection(itemId, std::move(title), std::chrono::seconds{0}, std::move(image), std::move(artist), std::move(tags)),
        items_(std::move(items)) {}

    void addMedia(std::shared_ptr<const Media> item) override {
        items_.push_back(std::move(item));
    }

    std::chrono::seconds getDuration() const override {
        std::chrono::seconds total{0};
        for (const auto& item : items_) {
            if (item) total += item->getDuration();
        }
        return total;
    }

    void Play() const override {
        // TODO
    }
};

class Playlist : public MediaCollection {
private:
    std::string ownerUsername_;
    std::vector<std::shared_ptr<const Media>> items_;

public:
    Playlist(int itemId, std::string title, std::string image, std::string artist, std::vector<std::shared_ptr<const Tag>> tags)
        : MediaCollection(itemId, std::move(title), std::chrono::seconds{0}, std::move(image), std::move(artist), std::move(tags)) {}

    void addMedia(std::shared_ptr<const Media> item) override {
        items_.push_back(std::move(item));
    }

    size_t getItemCount() const override { return items_.size(); }

    std::chrono::seconds getDuration() const override {
        std::chrono::seconds total{0};
        for (const auto& item : items_) {
            if (item) total += item->getDuration();
        }
        return total;
    }

    void Play() const override {
        // TODO
    }
};



