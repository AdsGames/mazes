#pragma once

#include <asw/asw.h>
#include <array>
#include <string>

class Button {
 public:
  Button() = default;

  void SetImages(const std::string& image1, const std::string& image2);

  void draw();

  void SetX(int newValue);
  void SetY(int newValue);

  void SetType(int newType);
  void SetValue(int newValue);

  bool Hover() const;

  int GetX() const;
  int GetY() const;

 private:
  int x{0};
  int y{0};
  int width{520};
  int height{120};

  std::array<asw::Texture, 2> images;
};
