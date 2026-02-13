#pragma once

#include <asw/asw.h>
#include <fstream>

#include "../button.h"
#include "../globals.h"
#include "./scenes.h"

class LevelSelectScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void init() override {
    back.SetImages("assets/images/buttons/back.png",
                   "assets/images/buttons/back_hover.png");

    background = asw::assets::loadTexture("assets/images/background.png");

    // Sets button positions
    back.SetX(380);
    back.SetY(40);

    // Load font
    font = asw::assets::loadFont("assets/fonts/dosis.ttf", 16);

    // Load sounds
    click = asw::assets::loadSample("assets/sfx/click.wav");

    // Load sprites
    levelSelect = asw::assets::loadTexture("assets/images/levelSelect.png");
    levelSelectLeft =
        asw::assets::loadTexture("assets/images/levelSelectLeft.png");
    levelSelectRight =
        asw::assets::loadTexture("assets/images/levelSelectRight.png");

    // Load tilemap
    tilemap.load("assets/levels/level" + std::to_string(currentLevel) + ".map");
  }

  void update(float deltaTime) override {
    Scene::update(deltaTime);

    // Go to menu
    if (asw::input::getKeyDown(asw::input::Key::Escape)) {
      sceneManager.setNextScene(GameState::Menu);
      return;
    }

    // Click buttons
    if (asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
      if (leftQuad.contains(asw::input::mouse.position) && currentLevel > 1) {
        std::string file_path =
            "assets/levels/level" + std::to_string(currentLevel - 1) + ".map";
        tilemap.load(file_path);
        asw::sound::play(click, 255, 125, false);
        currentLevel -= 1;
      }

      if (rightQuad.contains(asw::input::mouse.position)) {
        std::string file_path =
            "assets/levels/level" + std::to_string(currentLevel + 1) + ".map";

        std::ifstream read(file_path.c_str());
        if (!read.fail()) {
          tilemap.load(file_path);
          asw::sound::play(click, 255, 125, false);
          currentLevel += 1;
        }
      }

      if (playQuad.contains(asw::input::mouse.position)) {
        sceneManager.setNextScene(GameState::Game);
      }

      if (back.Hover() == true) {
        sceneManager.setNextScene(GameState::Menu);
      }
    }
  }

  void draw() override {
    asw::draw::sprite(levelSelect, asw::Vec2<float>(0, 0));
    back.draw();
    asw::draw::stretchSprite(background, playQuad);

    asw::draw::textCenter(font, "Level: " + std::to_string(currentLevel),
                          asw::Vec2<float>(640, 760),
                          asw::util::makeColor(0, 0, 0));

    asw::draw::textCenter(font, tilemap.levelText, asw::Vec2<float>(640, 800),
                          asw::util::makeColor(0, 0, 0));

    // Mini tiles tiles
    for (int i = 0; i < TileMap::WIDTH; i++) {
      for (int t = 0; t < TileMap::HEIGHT; t++) {
        auto& tile = tilemap.at(i, t);

        if (!tile.image || tile.value == 1) {
          continue;
        }

        if (perspective == 0) {
          auto position = asw::Quad<float>(i * 20 + 320, t * 20 + 220, 30, 30);
          asw::draw::stretchSprite(tile.image, position);
        } else if (perspective == 1) {
          auto position = asw::Quad<float>(i * 20 + 320, t * 20 + 220, 20, 20);
          asw::draw::stretchSprite(tile.image, position);
        }
      }
    }

    // Hovering
    if (leftQuad.contains(asw::input::mouse.position)) {
      asw::draw::sprite(levelSelectLeft, asw::Vec2<float>(0, 0));
    }
    if (rightQuad.contains(asw::input::mouse.position)) {
      asw::draw::sprite(levelSelectRight, asw::Vec2<float>(1080, 0));
    }
  }

 private:
  Button back;

  asw::Texture background;
  asw::Font font;
  asw::Sample click;

  asw::Texture levelSelect;
  asw::Texture levelSelectLeft;
  asw::Texture levelSelectRight;

  asw::Quad<float> leftQuad{0, 0, 200, 960};
  asw::Quad<float> rightQuad{1080, 0, 200, 960};
  asw::Quad<float> playQuad{320, 220, 640, 480};
};