#include "Menu.h"

#include <loadpng.h>
#include <functional>

#include "utility/AssetLoader.h"
#include "utility/MouseListener.h"
#include "utility/TransitionEffects.h"

Menu::Menu()
  : next_state(-1),
    show_help(false),
    btn_start(Button (380, 240)),
    btn_help(Button (380, 380)),
    btn_quit(Button (380, 520)) {

  // Load images
  menu = AssetLoader::LoadPng ("images/menu.png");
  cursor[0] = AssetLoader::LoadPng ("images/cursor1.png");
  cursor[1] = AssetLoader::LoadPng ("images/cursor2.png");
  help_menu = AssetLoader::LoadPng ("images/help.png");

  // Sets button images
  btn_start.SetImages ("images/buttons/start.png", "images/buttons/start_hover.png");
  btn_help.SetImages ("images/buttons/help.png", "images/buttons/help_hover.png");
  btn_quit.SetImages ("images/buttons/quit.png", "images/buttons/quit_hover.png");

  // Button onclicks
  btn_start.SetOnClick ([this]() {
    next_state = StateEngine::STATE_LEVEL_SELECT;
  });

  btn_help.SetOnClick ([this]() {
    show_help = true;
  });

  btn_quit.SetOnClick ([this]() {
    next_state = StateEngine::STATE_EXIT;
  });
}

Menu::~Menu() {
  TransitionEffects::highcolor_fade_out (16);

  destroy_bitmap(menu);
  destroy_bitmap(help_menu);
  destroy_bitmap(cursor[0]);
  destroy_bitmap(cursor[1]);
}

void Menu::draw (BITMAP *buffer) {
  //Draws Menu
  draw_sprite (buffer, menu, 0, 0);

  // Draws Buttons
  btn_start.Draw (buffer);
  btn_help.Draw (buffer);
  btn_quit.Draw (buffer);

  // Draw help
  if (show_help) {
    draw_sprite (buffer, help_menu, 0, 0);
  }

  //Draws Cursor
  draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
}

void Menu::update (StateEngine &engine) {
  // Change state
  if (next_state != -1) {
    engine.setNextState (next_state);
  }

  // Help closed
  if (!show_help) {
    btn_start.Update();
    btn_help.Update();
    btn_quit.Update();
  }
  // Close help
  else if (MouseListener::mouse_pressed & 1) {
    show_help = false;
  }
}

