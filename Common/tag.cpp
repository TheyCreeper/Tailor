#include "tag.h"
#include <string>

Tag::Tag(int tagId, std::string name, std::string color)
{
    TagId = tagId;
    Name = name;
    Color = color;
}

int Tag::getTagId() {
    return TagId;
}

std::string Tag::getName() {
    return Name;
}

std::string Tag::getColor() {
    return Color;
}
