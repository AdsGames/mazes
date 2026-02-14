#pragma once

#include <string>

#include "tile.h"

class TileMap {
public:
    static constexpr int WIDTH = 32;
    static constexpr int HEIGHT = 24;
    static constexpr int TILE_SIZE = 40;

    // Grid coordinate access
    Tile& at(int x, int y);

    // Pixel-to-grid coordinate access
    const Tile& atPixel(int pixelX, int pixelY) const;

    // Coordinate checking helpers
    int getValue(int x, int y) const;
    void setValue(int x, int y, int value);
    void setValue(Tile& tile, int value);

    // Map file loading
    bool load(const std::string& filename);

    int robots_captured { 0 };
    int robots_total { 0 };
    std::string level_text { "" };

private:
    Tile tiles[WIDTH][HEIGHT] {};

    asw::Texture robot;
    asw::Texture box;
    asw::Texture scrap;
    asw::Texture wall;
    asw::Texture garbagecan;
    asw::Texture janitorroom;
    asw::Texture wall2;
    asw::Texture janitorroomopen;
};
