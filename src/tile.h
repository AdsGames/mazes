#pragma once

#include <asw/asw.h>
#include <array>

// Tiles on screen
struct tile {
  asw::Texture image;
  int value;
  int dir;
};