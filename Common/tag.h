#ifndef TAG_H
#define TAG_H
#include <string>
class Tag
{
    int TagId;
    std::string Name;
    std::string Color;
public:
    Tag(int tagId, std::string name, std::string color);
    int getTagId();
    std::string getName();
    std::string getColor();
};

#endif // TAG_H
