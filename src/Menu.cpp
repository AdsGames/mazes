#include "Menu.h"

#include <loadpng.h>
#include <functional>

#include "utility/MouseListener.h"
#include "TransitionEffects.h"

Menu::Menu()
  : next_state(-1),
    show_help(false),
    start(Button (380, 240)),
    help(Button (380, 380)),
    quit(Button (380, 520)) {

  // Load images
  menu = load_png ("images/menu.png", nullptr);
  cursor[0] = load_png ("images/cursor1.png", nullptr);
  cursor[1] = load_png ("images/cursor2.png", nullptr);
  help_menu = load_png ("images/help.png", nullptr);

  // Sets button images
  start.SetImages ("images/buttons/start.png", "images/buttons/start_hover.png");
  help.SetImages ("images/buttons/help.png", "images/buttons/help_hover.png");
  quit.SetImages ("images/buttons/quit.png", "images/buttons/quit_hover.png");

  // Button onclicks
  start.SetOnClick ([this]() {
    next_state = StateEngine::STATE_LEVEL_SELECT;
  });

  help.SetOnClick ([this]() {
    show_help = true;
  });

  quit.SetOnClick ([this]() {
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
  start.Draw (buffer);
  help.Draw (buffer);
  quit.Draw (buffer);

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
    start.Update();
    help.Update();
    quit.Update();
  }
  // Close help
  else if (MouseListener::mouse_pressed & 1) {
    show_help = false;
  }
}

