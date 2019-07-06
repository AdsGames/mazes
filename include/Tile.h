#ifndef TILE_H
#define TILE_H

#include <allegro.h>
#include <string>

class Tile {

  public:
    Tile();
    Tile(int type);
    ~Tile();

    int GetY() const;
    int GetX() const;

    void Draw (BITMAP *buffer);

  private:
    void SetType(int type);
    void SetImage (std::string imagee);
    void SetX (int x);
    void SetY (int y);

    int x, y;
    int type;

    BITMAP *image;
};

#endif

