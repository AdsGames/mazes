#include "Menu.h"

#include <loadpng.h>

#include "TransitionEffects.h"

Menu::Menu() {
  // Load images
  menu = load_png ("images/menu.png", nullptr);
  cursor[0] = load_png ("images/cursor1.png", nullptr);
  cursor[1] = load_png ("images/cursor2.png", nullptr);
  help_menu = load_png ("images/help.png", nullptr);

  // Sets button images
  start.SetImages ("images/buttons/start.png", "images/buttons/start_hover.png");
  help.SetImages ("images/buttons/help.png", "images/buttons/help_hover.png");
  quit.SetImages ("images/buttons/quit.png", "images/buttons/quit_hover.png");

  // Sets button positions
  start.SetX (380);
  help.SetX (380);
  quit.SetX (380);

  start.SetY (240);
  help.SetY (380);
  quit.SetY (520);
}

Menu::~Menu() {

}

void Menu::draw (BITMAP *buffer) {
  if (mouse_b & 1) {
    if (help.Hover()) {
      draw_sprite (buffer, help_menu, 0, 0);
    }
  }

  //Draws Menu
  draw_sprite (buffer, menu, 0, 0);

  //Draws Buttons
  start.draw (buffer);
  help.draw (buffer);
  quit.draw (buffer);

  //Draws Cursor
  draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
}

void Menu::update (StateEngine &engine) {
  //Checks for mouse press
  if (mouse_b & 1) {
    if (start.Hover()) {
      TransitionEffects::highcolor_fade_out (16);
      engine.setNextState (StateEngine::STATE_LEVEL_SELECT);
    }
    else if (help.Hover()) {
      TransitionEffects::highcolor_fade_in (help_menu, 16);
      TransitionEffects::highcolor_fade_out (16);
    }
    else if (quit.Hover()) {
      engine.setNextState (StateEngine::STATE_EXIT);
    }
  }
}

