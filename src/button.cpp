#include "button.h"

#include <loadpng.h>

Button::Button() : x(0), y(0), width(520), height(120) {
  images[0] = nullptr;
  images[1] = nullptr;
}

Button::~Button() {}

void Button::SetImages(const char* image1, const char* image2) {
  images[0] = load_png(image1, nullptr);
  images[1] = load_png(image2, nullptr);
}

bool Button::Hover() const {
  return (mouse_x > GetX() && mouse_x < GetX() + width && mouse_y > GetY() &&
          mouse_y < GetY() + height);
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

void Button::draw(BITMAP* tempBitmap) {
  draw_sprite(tempBitmap, images[Hover()], GetX(), GetY());
}
