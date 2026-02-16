#pragma once

#include <array>
#include <asw/asw.h>
#include <string>

class Button : public asw::game::GameObject {
public:
    using asw::game::GameObject::GameObject;

    void setImages(const std::string& image1, const std::string& image2);

    void draw() override;

    bool hover() const;

private:
    std::array<asw::Texture, 2> images;
};
