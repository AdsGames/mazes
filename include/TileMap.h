#ifndef TILEMAP_H
#define TILEMAP_H

#include <string>

#include "Tile.h"

#define MAP_WIDTH 32
#define MAP_HEIGHT 24

class TileMap {
  public:
    TileMap();
    virtual ~TileMap();

    bool LoadMap(std::string file);
    void Draw(BITMAP *buffer);

  private:
    Tile tiles[MAP_WIDTH][MAP_HEIGHT];

    std::string level_name;
    int num_robots;
};

#endif // TILEMAP_H
