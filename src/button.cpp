#include "button.h"

#include <asw/asw.h>

void Button::SetImages(const std::string& image1, const std::string& image2) {
  images[0] = asw::assets::loadTexture(image1);
  images[1] = asw::assets::loadTexture(image2);
}

bool Button::Hover() const {
  return (
      asw::input::mouse.x > GetX() && asw::input::mouse.x < GetX() + width &&
      asw::input::mouse.y > GetY() && asw::input::mouse.y < GetY() + height);
}

void Button::SetX(int newValue) {
  x = newValue;
}

void Button::SetY(int newValue) {
  y = newValue;
}

int Button::GetX() const {
  return x;
}

int Button::GetY() const {
  return y;
}

void Button::draw() {
  auto frame = Hover() ? 1 : 0;
  asw::draw::sprite(images[frame], asw::Vec2<float>(x, y));
}
