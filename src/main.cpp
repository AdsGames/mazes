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
int main()
{
    using enum GameState;

    // Initializing
    asw::core::init(1280, 960);
    asw::display::setTitle("Mazes");
    asw::display::setIcon("assets/mazes.ico");

    // Starts Game
    auto app = asw::scene::SceneManager<GameState>();
    app.registerScene<GameScene>(Game, app);
    app.registerScene<MenuScene>(Menu, app);
    app.registerScene<WinScene>(Win, app);
    app.registerScene<LevelSelectScene>(LevelSelect, app);
    app.registerScene<IntroScene>(Intro, app);
    app.setNextScene(Intro);
    app.start();

    return 0;
}
