/*
 *  Allan Legemaate
 *  2012
 *  Mazes Main
 */

#include <asw/asw.h>

#include "./scenes/game.h"
#include "./scenes/init.h"
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
    asw::core::print_info();

    // Starts Game
    auto app = asw::scene::SceneManager<GameState>();
    app.register_scene<InitScene>(Init, app);
    app.register_scene<GameScene>(Game, app);
    app.register_scene<MenuScene>(Menu, app);
    app.register_scene<WinScene>(Win, app);
    app.register_scene<LevelSelectScene>(LevelSelect, app);
    app.register_scene<IntroScene>(Intro, app);
    app.set_next_scene(Init);
    app.start();

    return 0;
}
