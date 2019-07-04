#include "Intro.h"

#include <loadpng.h>

#include "TransitionEffects.h"

Intro::Intro() {
  intro = load_png ("images/intro.png", nullptr);
  splash = load_png ("images/splash.png", nullptr);
}

Intro::~Intro() {
  destroy_bitmap (intro);
  destroy_bitmap (splash);
}

void Intro::draw (BITMAP *buffer) {

}

void Intro::update (StateEngine &engine) {
  TransitionEffects::highcolor_fade_in (intro, 16);
  rest (1000);
  TransitionEffects::highcolor_fade_out (16);
  TransitionEffects::highcolor_fade_in (splash, 16);
  rest (1000);
  TransitionEffects::highcolor_fade_out (16);
  engine.setNextState (StateEngine::STATE_MENU);
}
