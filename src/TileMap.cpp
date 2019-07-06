#include "TileMap.h"

#include <fstream>
#include <regex>
#include <iostream>

TileMap::TileMap() {
  //ctor
}

TileMap::~TileMap() {
  //dtor
}

bool TileMap::LoadMap(std::string file) {
  std::ifstream read (file);

  if (read.fail())
    return false;

  std::string token = "";
  std::string tag = "";
  level_name = "";
  num_robots = 0;
  std::regex tag_pattern ("\\[.*\\]");

  int i = 0;
  int t = 0;

  while (!read.eof()) {
    // Get current tag
    read >> token;

    if (std::regex_match (token, tag_pattern)) {
      tag = token;
      read >> token;
    }

    // Get title
    if (!tag.compare ("[title]")) {
      level_name += token + " ";
    }

    // Get #robots
    if (!tag.compare ("[robots]")) {
      num_robots = std::stoi (token);
    }

    // Get map
    if (!tag.compare ("[map]")) {
      tiles[i][t] = Tile(std::stoi (token));
      std::cout << tag << " = " <<   i << std::endl;
      i++;

      if (i >= 24) {
        t++;
        i = 0;
      }
    }
  }

  read.close();
  return true;
}

void TileMap::Draw(BITMAP *buffer) {
  for (int i = 0; i < MAP_WIDTH; i++) {
    for (int t = 0; t < MAP_HEIGHT; t++) {
      tiles[i][t].Draw(buffer);
    }
  }
}
