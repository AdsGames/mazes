#include "Tile.h"

#include <loadpng.h>

Tile::Tile()
  : x (0), y (0), type (0), frame (0), selected (false), image (nullptr), sound (nullptr) {

}

Tile::~Tile() {
  destroy_bitmap (image);
  destroy_sample (sound);
}

void Tile::SetImage (const char *image) {
  this -> image = load_png (image, nullptr);
}

int Tile::GetX() const {
  return x;
}

int Tile::GetY() const {
  return y;
}

void Tile::SetX (int newValue) {
  x = newValue;
}

void Tile::SetY (int newValue) {
  y = newValue;
}

void Tile::draw (BITMAP *buffer) {
  draw_sprite (buffer, image, GetX(), GetY());
}
