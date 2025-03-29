#include "block.h"

void Block::SetImages(const std::string& path1, const std::string& path2) {
  images[0] = asw::assets::loadTexture(path1);
  images[1] = asw::assets::loadTexture(path2);
}

int Block::GetX() const {
  return x;
}

int Block::GetY() const {
  return y;
}

void Block::SetX(int newValue) {
  x = newValue;
}

void Block::SetY(int newValue) {
  y = newValue;
}

void Block::draw() {
  if (frame <= 4) {
    asw::draw::sprite(images[0], asw::Vec2<float>(x, y));
  } else {
    asw::draw::sprite(images[1], asw::Vec2<float>(x, y));
  }

  frame += (frame + 1) % 16;
}
