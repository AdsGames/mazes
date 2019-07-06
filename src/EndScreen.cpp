#include "EndScreen.h"

#include <loadpng.h>

#include "utility/TransitionEffects.h"
#include "globals.h"

EndScreen::EndScreen() {
  winscreen = load_png ("images/winscreen.png", nullptr);
}

EndScreen::~EndScreen() {
  destroy_bitmap (winscreen);
}

void EndScreen::draw (BITMAP *buffer) {
  draw_sprite (buffer, winscreen, 0, 0);
  textprintf_ex (buffer, font, 310, 400, makecol (0, 0, 0), -1, "%i", score);
}

void EndScreen::update (StateEngine &engine) {
  if (key[KEY_SPACE] || key[KEY_ENTER] || mouse_b & 1  || joy[0].button[0].b) {
    TransitionEffects::FadeOut (16);
    engine.setNextState (StateEngine::STATE_MENU);
    TransitionEffects::FadeIn (winscreen, 16);
  }
}
