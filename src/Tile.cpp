#include "Tile.h"

#include "utility/AssetLoader.h"

Tile::Tile()
  : x (0), y (0), type (0), image (nullptr) {

}

Tile::Tile(int type)
  : Tile() {
  SetType(type);
}

Tile::~Tile() {
  destroy_bitmap (image);
}

int Tile::GetX() const {
  return x;
}

int Tile::GetY() const {
  return y;
}

void Tile::SetType(int type) {

}

void Tile::SetImage (std::string image) {
  this -> image = AssetLoader::LoadPng (image);
}

void Tile::SetX (int x) {
  this -> x = x;
}

void Tile::SetY (int y) {
  this -> y = y;
}

void Tile::Draw (BITMAP *buffer) {
  draw_sprite (buffer, image, x, y);
}
