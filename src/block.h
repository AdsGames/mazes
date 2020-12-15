#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>

class Block {
 public:
  Block();
  ~Block();

  void SetImages(const char* image1, const char* image2);

  int GetY() const;
  int GetX() const;

  void SetX(int newValue);
  void SetY(int newValue);

  void draw(BITMAP* buffer);

 private:
  int x, y;

  int type;

  int frame;

  bool selected;

  BITMAP* images[2];
  SAMPLE* sound;
};

#endif
