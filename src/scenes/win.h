
#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "../globals.h"
#include "./scenes.h"

class WinScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void init() override {
    winscreen = asw::assets::loadTexture("assets/images/winscreen.png");
    font = asw::assets::loadFont("assets/fonts/dosis.ttf", 16);
  }

  void update(float deltaTime) override {
    Scene::update(deltaTime);

    if (asw::input::getKeyDown(asw::input::Key::Space) ||
        asw::input::getKeyDown(asw::input::Key::Return) ||
        asw::input::getKeyDown(asw::input::Key::Escape) ||
        asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
      sceneManager.setNextScene(GameState::Menu);
    }
  }

  void draw() override {
    asw::draw::sprite(winscreen, asw::Vec2<float>(0, 0));
    asw::draw::text(font, std::to_string(score), asw::Vec2<float>(310, 400),
                    asw::util::makeColor(0, 0, 0));
  }

 private:
  asw::Texture winscreen;
  asw::Font font;
};