#include "Intro.h"

#include <loadpng.h>

#include "utility/AssetLoader.h"
#include "utility/TransitionEffects.h"

Intro::Intro() {
  intro = AssetLoader::LoadPng("images/intro.png");
  splash = AssetLoader::LoadPng ("images/splash.png");

  tmr.Start();
  TransitionEffects::highcolor_fade_in (intro, 16);
}

Intro::~Intro() {
  destroy_bitmap (intro);
  destroy_bitmap (splash);
  TransitionEffects::highcolor_fade_out (16);
}

void Intro::draw (BITMAP *buffer) {
  if (tmr.GetElapsedTime<milliseconds>() > 1000) {
    draw_sprite (buffer, splash, 0, 0);
  }
  else if (tmr.GetElapsedTime<milliseconds>() > 0) {
    draw_sprite (buffer, intro, 0, 0);
  }
}

void Intro::update (StateEngine &engine) {
  if (key[KEY_SPACE] || tmr.GetElapsedTime<milliseconds>() > 2000) {
    engine.setNextState (StateEngine::STATE_MENU);
  }
}
