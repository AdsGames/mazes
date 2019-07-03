#ifndef TILE_H
#define TILE_H

#include <allegro.h>

class Tile {

  public:
    Tile();
    ~Tile();

    void SetImage (const char *image);

    int GetY() const;
    int GetX() const;

    void SetX (int newValue);
    void SetY (int newValue);

    void draw (BITMAP *buffer);

  private:

    int x, y;

    int type;

    int frame;

    bool selected;

    BITMAP *image;
    SAMPLE *sound;
};

#endif

