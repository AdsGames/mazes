#pragma once

#include <asw/asw.h>
#include <array>
#include <string>

class Block {
 public:
  Block() = default;

  void SetImages(const std::string& path1, const std::string& path2);

  int GetY() const;
  int GetX() const;

  void SetX(int newValue);
  void SetY(int newValue);

  void draw();

 private:
  int x{0};
  int y{0};

  int frame{0};

  std::array<asw::Texture, 2> images;
  asw::Sample sound;
};
