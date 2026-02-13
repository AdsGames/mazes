/*
 *  Allan Legemaate
 *  2012
 *  Mazes Main
 */

#include <asw/asw.h>

#include "./scenes/game.h"
#include "./scenes/intro.h"
#include "./scenes/level_select.h"
#include "./scenes/menu.h"
#include "./scenes/scenes.h"
#include "./scenes/win.h"

// Main functions run here
int main() {
  // Initializing
  asw::core::init(1280, 960);
  asw::display::setTitle("Mazes");

  // Starts Game
  auto app = asw::scene::SceneManager<GameState>();
  app.registerScene<GameScene>(GameState::Game, app);
  app.registerScene<MenuScene>(GameState::Menu, app);
  app.registerScene<WinScene>(GameState::Win, app);
  app.registerScene<LevelSelectScene>(GameState::LevelSelect, app);
  app.registerScene<IntroScene>(GameState::Intro, app);
  app.setNextScene(GameState::Intro);
  app.start();

  return 0;
}
