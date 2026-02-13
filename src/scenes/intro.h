
#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "./scenes.h"

class IntroScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void init() override {
    intro = asw::assets::loadTexture("assets/images/intro.png");
    splash = asw::assets::loadTexture("assets/images/splash.png");
  }

  void update(float deltaTime) override { Scene::update(deltaTime); }

  void draw() override {
    asw::display::clear();
    asw::draw::sprite(intro, asw::Vec2<float>(0, 0));
    asw::display::present();
    // rest(1000);
    asw::display::clear();
    asw::draw::sprite(splash, asw::Vec2<float>(0, 0));
    asw::display::present();
    // rest(1000);

    sceneManager.setNextScene(GameState::Menu);

    return;
  }

 private:
  asw::Texture intro;
  asw::Texture splash;
};