#include "button.h"

#include <asw/asw.h>

using namespace asw::assets;
using namespace asw::input;
using namespace asw::draw;

void Button::SetImages(const std::string& image1, const std::string& image2)
{
    images[0] = loadTexture(image1);
    images[1] = loadTexture(image2);
}

bool Button::Hover() const
{
    return (mouse.position.x > x && mouse.position.x < x + width && mouse.position.y > y
        && mouse.position.y < y + height);
}

void Button::SetX(int newValue)
{
    x = newValue;
}

void Button::SetY(int newValue)
{
    y = newValue;
}

int Button::GetX() const
{
    return x;
}

int Button::GetY() const
{
    return y;
}

void Button::draw()
{
    auto frame = Hover() ? 1 : 0;
    sprite(images[frame], asw::Vec2<float>(x, y));
}
