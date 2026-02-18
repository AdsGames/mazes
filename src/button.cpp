#include "button.h"

#include "./colors.h"

bool Button::hover() const
{
    return transform.contains(asw::input::mouse.position);
}

void Button::draw()
{

    const auto text_height = asw::util::getTextSize(font, text).y;
    const auto text_offset = static_cast<float>(text_height) / 2.0F;

    const auto text_position
        = transform.position + transform.size / 2 - asw::Vec2<float>(0, text_offset);

    if (hover()) {
        if (hover_texture != nullptr) {
            asw::draw::stretchSprite(hover_texture, transform);
        } else {
            asw::draw::rectFill(transform, hover_color);
        }
    } else {
        if (texture != nullptr) {
            asw::draw::stretchSprite(texture, transform);
        } else {
            asw::draw::rectFill(transform, normal_color);
        }
    }

    if (!text.empty()) {
        asw::draw::text(font, text, text_position, palette::white, asw::TextJustify::CENTER);
    }
}
