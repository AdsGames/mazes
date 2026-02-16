#include "button.h"

#include <asw/asw.h>

using namespace asw::assets;
using namespace asw::input;
using namespace asw::draw;

void Button::setImages(const std::string& image1, const std::string& image2)
{
    images[0] = loadTexture(image1);
    images[1] = loadTexture(image2);

    transform.size = asw::Vec2<float>(images[0]->w, images[0]->h);
}

bool Button::hover() const
{
    return transform.contains(asw::input::mouse.position);
}

void Button::draw()
{
    auto frame = hover() ? 1 : 0;
    sprite(images[frame], transform.position);
}
