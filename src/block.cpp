#include "block.h"

#include <loadpng.h>

Block::Block()
  : x (0), y (0), type (0), frame (0), selected (false), sound (nullptr) {

  images[0] = nullptr;
  images[1] = nullptr;
}

Block::~Block() {
  destroy_bitmap (images[0]);
  destroy_bitmap (images[1]);
  destroy_sample (sound);
}

void Block::SetImages (const char *image1, const char *image2) {
  images[0] = load_png (image1, nullptr);
  images[1] = load_png (image2, nullptr);
}

int Block::GetX() const {
  return x;
}

int Block::GetY() const {
  return y;
}

void Block::SetX (int newValue) {
  x = newValue;
}

void Block::SetY (int newValue) {
  y = newValue;
}

void Block::draw (BITMAP *buffer) {
  if (frame == 0 || frame == 1 || frame == 2 || frame == 3 || frame == 4) {
    frame += 1;
    draw_sprite (buffer, images[0], GetX(), GetY());
  }
  else if (frame == 5 || frame == 6 || frame == 7 || frame == 8 || frame == 9 || frame == 10 || frame == 11 || frame == 12 || frame == 13 || frame == 14 || frame == 15) {
    frame += 1;
    draw_sprite (buffer, images[1], GetX(), GetY());

    if (frame == 16) {
      frame = 0;
    }
  }
}
