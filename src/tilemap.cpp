#include "tilemap.h"

#include <fstream>
#include "globals.h"
#include "id.h"

using namespace asw::assets;

tile& TileMap::at(int x, int y) {
  return tiles[x][y];
}

const tile& TileMap::atPixel(int pixelX, int pixelY) const {
  return tiles[pixelX / TILE_SIZE][pixelY / TILE_SIZE];
}

int TileMap::getValue(int x, int y) const {
  return tiles[x][y].value;
}

void TileMap::setValue(int x, int y, int value) {
  setValue(tiles[x][y], value);
}

void TileMap::setValue(tile& tile, int value) {
  tile.value = value;

  // Assign images to data values
  if (tile.value == V_ROBOT) {
    tile.image = robot;
  } else if (tile.value == V_BOX) {
    tile.image = box;
  } else if (tile.value == V_SCRAP) {
    tile.image = scrap;
  } else if (tile.value == V_WALL) {
    tile.image = wall;
  } else if (tile.value == V_GARBAGECAN) {
    tile.image = garbagecan;
  } else if (tile.value == V_JANITORROOM) {
    tile.image = janitorroom;
  } else if (tile.value == V_WALL2) {
    tile.image = wall2;
  } else if (tile.value == V_JANITORROOMOPEN) {
    tile.image = janitorroomopen;
  } else {
    tile.image = nullptr;
  }
}

bool TileMap::load(const std::string& filename) {
  if (perspective == 0) {
    wall = loadTexture("assets/images/blocks/3d/wall.png");
    wall2 = loadTexture("assets/images/blocks/3d/wall2.png");
    robot = loadTexture("assets/images/blocks/3d/robot.png");
    box = loadTexture("assets/images/blocks/3d/box.png");
    scrap = loadTexture("assets/images/blocks/3d/scrap.png");
    garbagecan = loadTexture("assets/images/blocks/3d/garbagecan.png");
    janitorroom = loadTexture("assets/images/blocks/3d/janitor_room.png");
    janitorroomopen =
        loadTexture("assets/images/blocks/3d/janitor_room_open.png");
  } else {
    wall = loadTexture("assets/images/blocks/2d/wall.png");
    wall2 = loadTexture("assets/images/blocks/2d/wall2.png");
    robot = loadTexture("assets/images/blocks/2d/robot.png");
    box = loadTexture("assets/images/blocks/2d/box.png");
    scrap = loadTexture("assets/images/blocks/2d/scrap.png");
    garbagecan = loadTexture("assets/images/blocks/2d/garbagecan.png");
    janitorroom = loadTexture("assets/images/blocks/2d/janitor_room.png");
    janitorroomopen =
        loadTexture("assets/images/blocks/2d/janitor_room_open.png");
  }

  std::ifstream read(filename.c_str());
  if (read.fail()) {
    return false;
  }

  // 1) Title line (whole line, including spaces)
  std::getline(read, levelText);
  if (read.fail()) {
    return false;
  }

  // 2) Required robotsLeft (next integer)
  read >> robotsLeft;
  robotsTotal = robotsLeft;
  if (read.fail()) {
    return false;
  }

  // IMPORTANT: consume the newline after the integer so subsequent reads are
  // clean
  read.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  // 3) Tile values (next 24 lines, each with 32 integers)
  for (int y = 0; y < HEIGHT; y++) {
    for (int x = 0; x < WIDTH; x++) {
      int value;
      read >> value;

      if (read.fail()) {
        return false;
      }

      setValue(tiles[x][y], value);
    }
  }

  return true;
}
