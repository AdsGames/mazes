#pragma once

#include <array>
#include <asw/asw.h>
#include <string>

class Button : public asw::game::GameObject {
public:
    using asw::game::GameObject::GameObject;

    void setFont(const asw::Font& font)
    {
        this->font = font;
    }

    void setText(const std::string& text)
    {
        this->text = text;
    }

    void setTransform(const asw::Quad<float>& transform)
    {
        this->transform = transform;
    }

    void setColor(const asw::Color& normal_color)
    {
        this->normal_color = normal_color;
        this->hover_color = asw::Color::darken(normal_color, 0.2F);
    }

    void setImages(const asw::Texture& normal_texture, const asw::Texture& hover_texture)
    {
        this->texture = normal_texture;
        this->hover_texture = hover_texture;

        this->transform.size.x = static_cast<float>(normal_texture->w);
        this->transform.size.y = static_cast<float>(normal_texture->h);
    }

    void draw() override;

    bool hover() const;

private:
    asw::Texture texture;
    asw::Texture hover_texture;

    asw::Color normal_color;
    asw::Color hover_color;
    std::string text;
    asw::Font font;
};
