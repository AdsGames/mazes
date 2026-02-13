#pragma once

#include <asw/asw.h>
#include <array>
#include <cmath>
#include <ctime>
#include <sstream>
#include <string>

#include "../button.h"
#include "../id.h"

#include "../globals.h"
#include "./scenes.h"

class GameScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void init() override {
    using namespace asw::assets;
    using namespace asw::sound;

    timeAcc = 0.0F;
    moveAcc = 0.0F;
    paused = false;
    lives = 3;
    position = asw::Vec2<int>(40, 40);
    characterRotation = 0;
    hasBroom = false;
    won = false;
    isDoneLevel = false;

    font = loadFont("assets/fonts/dosis.ttf", 16);

    if (perspective == 0) {
      character[0] = loadTexture("assets/images/blocks/3d/character_down.png");
      character[1] = loadTexture("assets/images/blocks/3d/character_up.png");
      character[2] = loadTexture("assets/images/blocks/3d/character_left.png");
      character[3] = loadTexture("assets/images/blocks/3d/character_right.png");
      broom = loadTexture("assets/images/blocks/3d/broom.png");
      wood_floor = loadTexture("assets/images/blocks/3d/wood_floor.png");
    }

    if (perspective == 1) {
      character[0] = loadTexture("assets/images/blocks/2d/character.png");
      character[1] = loadTexture("assets/images/blocks/2d/character.png");
      character[2] = loadTexture("assets/images/blocks/2d/character.png");
      character[3] = loadTexture("assets/images/blocks/2d/character.png");

      broom = loadTexture("assets/images/blocks/2d/broom.png");
      wood_floor = loadTexture("assets/images/blocks/2d/wood_floor.png");
    }

    background = loadTexture("assets/images/background.png");

    // Sounds and music
    sweep = loadSample("assets/sfx/sweep.wav");
    explode = loadSample("assets/sfx/explode.wav");
    trash = loadSample("assets/sfx/trash.wav");
    oof = loadSample("assets/sfx/oof.wav");
    winsound = loadSample("assets/sfx/winsound.wav");
    door = loadSample("assets/sfx/door.wav");
    hitwall = loadSample("assets/sfx/hitwall.wav");
    boxhitwall = loadSample("assets/sfx/boxhitwall.wav");
    boxslide = loadSample("assets/sfx/boxslide.wav");

    song = loadMusic("assets/sfx/music.ogg");

    currentLevel = 1;
    score = 0;
    tilemap.levelText = "";

    // Background Music
    playMusic(song, 255);
  }

  void update(float deltaTime) override {
    Scene::update(deltaTime);

    if (paused && asw::input::getKeyDown(asw::input::Key::M)) {
      sceneManager.setNextScene(GameState::Menu);
    }

    // Pause Game
    if (asw::input::getKeyDown(asw::input::Key::Escape)) {
      paused = !paused;
      return;
    }

    // Ticker for character movement and robot movement
    timeAcc += deltaTime;
    moveAcc += deltaTime;
    if (timeAcc >= 500) {
      timeAcc -= 500;
      updateRobots();
    }

    // Character movement
    characterMove();

    // Die
    if (tilemap.atPixel(position.x, position.y).value == V_ROBOT) {
      lives -= 1;
      position.x = 40;
      position.y = 40;
      asw::sound::play(oof);
    }

    // Restart Map
    if (asw::input::getKeyDown(asw::input::Key::R)) {
      changeMap();
    }

    // Finish Level
    if (isDoneLevel) {
      asw::sound::play(winsound);
      currentLevel += 1;
      lives += 1;
      changeMap();
    }

    // Die
    if (lives <= 0) {
      sceneManager.setNextScene(GameState::Menu);
    }
  }

  void draw() override {
    using namespace asw::draw;

    // Draw background
    sprite(background, asw::Vec2<float>(0, 0));

    // Draws Tiles
    for (int t = 0; t < TileMap::HEIGHT; t++) {
      for (int i = 0; i < TileMap::WIDTH; i++) {
        if (position.x / 40 == i && position.y / 40 == t) {
          // Draws Character
          if (perspective == 0) {
            if (characterRotation == 0) {
              sprite(character[0], asw::Vec2<float>(position.x, position.y));
            } else if (characterRotation == 128) {
              sprite(character[1], asw::Vec2<float>(position.x, position.y));
            } else if (characterRotation == 64) {
              sprite(character[2], asw::Vec2<float>(position.x, position.y));
            } else if (characterRotation == 192) {
              sprite(character[3], asw::Vec2<float>(position.x, position.y));
            }
          } else {
            sprite(character[0], asw::Vec2<float>(position.x, position.y));
          }

          // Draws broom if needed
          if (asw::input::getKey(asw::input::Key::Space) && hasBroom == true) {
            if (perspective == 0) {
              rotateSprite(broom,
                           asw::Vec2<float>(position.x + 10, position.y + 10),
                           characterRotation);
            } else {
              sprite(broom, asw::Vec2<float>(position.x, position.y));
            }
          }
        }

        if (perspective == 0) {
          stretchSprite(tilemap.at(i, t).image,
                        asw::Quad<float>(i * 40, t * 40, 60, 60));
        } else {
          stretchSprite(tilemap.at(i, t).image,
                        asw::Quad<float>(i * 40, t * 40, 40, 40));
        }
      }
    }

    // Draws Stats
    rectFill(asw::Quad<float>(0, 0, 1280, 20),
             asw::util::makeColor(0, 0, 0, 128));

    text(font, "Score:" + std::to_string(score), asw::Vec2<float>(0, 0),
         asw::util::makeColor(255, 255, 255));

    // Robot progress meter
    {
      const float meterWidth = 150.0F;
      const float meterHeight = 12.0F;
      const float meterX = 1280.0F - meterWidth - 5.0F;
      const float meterY = 4.0F;

      const float progress =
          tilemap.robotsTotal > 0
              ? 1.0F - static_cast<float>(tilemap.robotsLeft) /
                           static_cast<float>(tilemap.robotsTotal)
              : 1.0F;

      // Background
      rectFill(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight),
               asw::util::makeColor(60, 60, 60));

      // Filled portion (red when robots remain, green when cleared)
      const auto barColor = tilemap.robotsLeft > 0
                                ? asw::util::makeColor(200, 50, 50)
                                : asw::util::makeColor(50, 200, 50);
      rectFill(
          asw::Quad<float>(meterX, meterY, meterWidth * progress, meterHeight),
          barColor);

      // Border
      rect(asw::Quad<float>(meterX, meterY, meterWidth, meterHeight),
           asw::util::makeColor(255, 255, 255));

      // Label
      textRight(font,
                "Robots:" + std::to_string(tilemap.robotsLeft) + "/" +
                    std::to_string(tilemap.robotsTotal),
                asw::Vec2<float>(meterX - 5.0F, 0),
                asw::util::makeColor(255, 255, 255));
    }

    textCenter(font, "Lives:" + std::to_string(lives), asw::Vec2<float>(640, 0),
               asw::util::makeColor(255, 255, 255));

    // Pause Game
    if (paused) {
      rectFill(asw::Quad<float>(200, 200, 880, 560),
               asw::util::makeColor(255, 255, 255, 128));
      textCenter(font, "Paused press ESC to resume. Press M to go to the Menu.",
                 asw::Vec2<float>(640, 480), asw::util::makeColor(0, 0, 0));
    }
  }

 private:
  // Controls Character Movements
  void characterMove() {
    using namespace asw::input;

    if (getKey(Key::Up) || getKey(Key::W)) {
      characterRotation = 128;
      moveTowards(0, -1);
    } else if (getKey(Key::Down) || getKey(Key::S)) {
      characterRotation = 0;
      moveTowards(0, 1);
    } else if (getKey(Key::Left) || getKey(Key::A)) {
      characterRotation = 64;
      moveTowards(-1, 0);
    } else if (getKey(Key::Right) || getKey(Key::D)) {
      characterRotation = 192;
      moveTowards(1, 0);
    }
  }

  void moveTowards(int dx, int dy) {
    using namespace asw::input;

    if (moveAcc < 100) {
      return;
    }

    moveAcc = 0;

    auto& tile = tilemap.at(position.x / 40 + dx, position.y / 40 + dy);
    auto& nextTile =
        tilemap.at(position.x / 40 + dx * 2, position.y / 40 + dy * 2);

    // Allow walking into
    if (tile.value == V_EMPTY || tile.value == V_WALL2 ||
        tile.value == V_ROBOT) {
      position.x += dx * 40;
      position.y += dy * 40;
      return;
    }

    if (tile.value == V_WALL) {
      asw::sound::play(hitwall);
      return;
    }

    if (tile.value == V_BOX && nextTile.value == V_EMPTY) {
      asw::sound::play(boxslide);

      tilemap.setValue(tile, V_EMPTY);
      tilemap.setValue(nextTile, V_BOX);
      position.x += dx * 40;
      position.y += dy * 40;
      return;
    }

    if (tile.value == V_BOX && nextTile.value != 0) {
      if (nextTile.value == V_ROBOT) {
        asw::sound::play(explode);
        tilemap.setValue(nextTile, V_SCRAP);
        score += 100;
      } else {
        asw::sound::play(boxhitwall);
      }

      return;
    }

    if (tile.value == V_SCRAP && nextTile.value == V_EMPTY &&
        hasBroom == true && getKey(Key::Space)) {
      tilemap.setValue(tile, V_EMPTY);
      tilemap.setValue(nextTile, V_SCRAP);
      asw::sound::play(sweep);

      return;
    }

    if (tile.value == V_SCRAP && nextTile.value == V_GARBAGECAN &&
        hasBroom == true && getKey(Key::Space)) {
      tilemap.setValue(tile, V_EMPTY);
      score += 50;

      if (tilemap.robotsLeft > 0) {
        tilemap.robotsLeft -= 1;
      }

      asw::sound::play(sweep);
      asw::sound::play(trash);

      return;
    }

    if (tile.value == V_JANITORROOM && !hasBroom) {
      asw::sound::play(door);
      hasBroom = true;
      tilemap.setValue(tile, V_JANITORROOMOPEN);
      return;
    }

    if (tile.value == V_JANITORROOMOPEN && tilemap.robotsLeft == 0) {
      isDoneLevel = true;
      return;
    }
  }

  void updateRobots() {
    const int playerTileX = position.x / 40;
    const int playerTileY = position.y / 40;

    // Collect all robot positions first to avoid double-processing
    asw::Vec2<int> robots[TileMap::WIDTH * TileMap::HEIGHT];
    int robotCount = 0;

    for (int i = 0; i < TileMap::WIDTH; i++) {
      for (int t = 0; t < TileMap::HEIGHT; t++) {
        if (tilemap.at(i, t).value == V_ROBOT) {
          robots[robotCount++] = {i, t};
        }
      }
    }

    for (int r = 0; r < robotCount; r++) {
      const int rx = robots[r].x;
      const int ry = robots[r].y;

      // Robot may have been destroyed since we collected positions
      if (tilemap.at(rx, ry).value != V_ROBOT) {
        continue;
      }

      // Build list of valid moves
      asw::Vec2<int> moves[4] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
      asw::Vec2<int> valid[4];
      int validCount = 0;

      for (const auto& m : moves) {
        int nx = rx + m.x;
        int ny = ry + m.y;
        if (nx >= 0 && nx < TileMap::WIDTH && ny >= 0 && ny < TileMap::HEIGHT &&
            tilemap.at(nx, ny).value == V_EMPTY) {
          valid[validCount++] = m;
        }
      }

      if (validCount == 0) {
        continue;
      }

      // 50% chance to move toward the player, 50% random
      asw::Vec2<int> chosen = valid[asw::random::between(0, validCount - 1)];

      if (asw::random::between(0, 1) == 0) {
        int bestDist = std::abs(rx + chosen.x - playerTileX) +
                       std::abs(ry + chosen.y - playerTileY);

        for (int v = 0; v < validCount; v++) {
          int dist = std::abs(rx + valid[v].x - playerTileX) +
                     std::abs(ry + valid[v].y - playerTileY);
          if (dist < bestDist) {
            bestDist = dist;
            chosen = valid[v];
          }
        }
      }

      tilemap.setValue(rx + chosen.x, ry + chosen.y, V_ROBOT);
      tilemap.setValue(rx, ry, V_EMPTY);
    }
  }

  // Change tiles
  void changeMap() {
    isDoneLevel = false;
    hasBroom = false;
    position = {40, 40};

    if (!tilemap.load("assets/levels/level" + std::to_string(currentLevel) +
                      ".map")) {
      sceneManager.setNextScene(GameState::Win);
    }
  }

  float timeAcc;
  float moveAcc;

  bool paused;

  asw::Font font;

  // Creates integers
  int lives;
  asw::Vec2<int> position;
  int characterRotation;

  bool hasBroom;
  bool won;
  bool isDoneLevel;

  // Define bitmaps
  std::array<asw::Texture, 4> character;
  asw::Texture background;
  asw::Texture ground;
  asw::Texture broom;
  asw::Texture wood_floor;

  // Define sound effects
  asw::Music song;
  asw::Sample hitwall;
  asw::Sample boxhitwall;
  asw::Sample boxslide;
  asw::Sample explode;
  asw::Sample sweep;
  asw::Sample trash;
  asw::Sample oof;
  asw::Sample winsound;
  asw::Sample door;
};