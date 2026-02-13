#pragma once

#include <asw/asw.h>

#include "../button.h"
#include "./scenes.h"

class MenuScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void init() override {
    // Sets button images
    start.SetImages("assets/images/buttons/start.png",
                    "assets/images/buttons/start_hover.png");
    help.SetImages("assets/images/buttons/help.png",
                   "assets/images/buttons/help_hover.png");
    quit.SetImages("assets/images/buttons/quit.png",
                   "assets/images/buttons/quit_hover.png");
    mode.SetImages("assets/images/buttons/mode_3d.png",
                   "assets/images/buttons/mode_3d_hover.png");

    // Sets button positions
    start.SetX(380);
    help.SetX(380);
    quit.SetX(380);
    mode.SetX(380);

    start.SetY(240);
    help.SetY(380);
    quit.SetY(520);
    mode.SetY(660);

    // Load font
    font = asw::assets::loadFont("assets/fonts/dosis.ttf", 16);

    // Load sprites
    menu = asw::assets::loadTexture("assets/images/menu.png");
    help_menu = asw::assets::loadTexture("assets/images/help.png");

    // Help open
    helpOpen = false;
  }

  void update(float deltaTime) override {
    Scene::update(deltaTime);

    if (helpOpen) {
      if (asw::input::keyboard.anyPressed ||
          asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
        helpOpen = false;
      }
    } else if (asw::input::getKeyDown(asw::input::Key::Escape)) {
      asw::core::exit = true;
    }

    // Checks for mouse press
    if (asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
      if (start.Hover()) {
        sceneManager.setNextScene(GameState::LevelSelect);
      } else if (help.Hover() && !helpOpen) {
        helpOpen = true;
      } else if (quit.Hover()) {
        asw::core::exit = true;
      } else if (mode.Hover()) {
        if (perspective == 0) {
          perspective = 1;
          mode.SetImages("assets/images/buttons/mode_2d.png",
                         "assets/images/buttons/mode_2d_hover.png");
        } else if (perspective == 1) {
          perspective = 0;
          mode.SetImages("assets/images/buttons/mode_3d.png",
                         "assets/images/buttons/mode_3d_hover.png");
        }
      }
    }
  }

  void draw() override {
    // Draws Menu
    asw::draw::sprite(menu, asw::Vec2<float>(0, 0));

    // Draws Buttons
    start.draw();
    help.draw();
    quit.draw();
    mode.draw();

    if (helpOpen) {
      asw::draw::sprite(help_menu, asw::Vec2<float>(0, 0));
    }
  }

 private:
  // Creates Buttons
  Button start;
  Button help;
  Button quit;
  Button mode;

  asw::Font font;

  asw::Texture help_menu;
  asw::Texture menu;

  // Help open
  bool helpOpen;
};