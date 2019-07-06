#include "LevelSelect.h"

#include <loadpng.h>
#include <fstream>
#include <functional>

#include "globals.h"
#include "utility/tools.h"
#include "utility/AssetLoader.h"
#include "utility/TransitionEffects.h"

// Create screen
LevelSelect::LevelSelect() {
  // Load images
  background = AssetLoader::LoadPng ("images/background.png");
  cursor[0] = AssetLoader::LoadPng ("images/cursor1.png");
  cursor[1] = AssetLoader::LoadPng ("images/cursor2.png");

  // Load sounds
  click = AssetLoader::LoadWav ("sfx/click.wav");

  // Create button
  btnBack = Button (380, 40);
  btnLeft = Button (0, 0);
  btnRight = Button (1080, 0);

  // Sets button images
  btnBack.SetImages ("images/buttons/back.png", "images/buttons/back_hover.png");
  btnLeft.SetImages ("images/buttons/levelSelectLeft.png", "images/buttons/levelSelectLeftHover.png");
  btnRight.SetImages ("images/buttons/levelSelectRight.png", "images/buttons/levelSelectRightHover.png");

  // On clicks
  btnBack.SetOnClick ([this]() {
    next_state = StateEngine::STATE_MENU;
  });

  btnLeft.SetOnClick ([this]() {
    play_sample (click, 255, 125, 1000, 0);
    levelOn = levelOn == 0 ? 0 : levelOn - 1;
    //setupGame (false);
  });

  btnRight.SetOnClick ([this]() {
    play_sample (click, 255, 125, 1000, 0);
    levelOn = levelOn == 8 ? 8 : levelOn + 1;
    //setupGame (false);
  });
}

// Destroy screen
LevelSelect::~LevelSelect() {
  TransitionEffects::FadeOut (16);
  destroy_bitmap (background);
  destroy_bitmap (cursor[0]);
  destroy_bitmap (cursor[1]);
  destroy_sample (click);
}

// Draw screen
void LevelSelect::draw (BITMAP *buffer) {
  clear_to_color (buffer, 0x0026FF);

  btnBack.Draw (buffer);
  btnLeft.Draw (buffer);
  btnRight.Draw (buffer);

  stretch_sprite (buffer, background, 320, 220, 640, 480);
  textprintf_centre_ex (buffer, font, 640, 760, makecol (0, 0, 0), makecol (255, 255, 255), "Level:%i", levelOn);
  draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
}

// Update screen
void LevelSelect::update (StateEngine &engine) {
  // Go to menu
  if (key[KEY_M]) {
    next_state = StateEngine::STATE_MENU;
  }

  if (next_state != -1) {
    engine.setNextState (next_state);
  }

  btnBack.Update();
  btnLeft.Update();
  btnRight.Update();

  //Mini tiles tiles
  /*for (int i = 0; i < 24; i++) {
    for (int t = 31; t >= 0; t--) {
      if (tiles[i][t].image[0] != NULL && tiles[i][t].value != 1) {
        if (perspective == 0) {
          stretch_sprite (buffer, tiles[i][t].image[0], t * 16 + 320 + i * 16, i * 8 - t * 8 + 220, 32, 64);
        }
        else if (perspective == 1) {
          stretch_sprite (buffer, tiles[i][t].image[0], t * 20 + 320, i * 20 + 220, 20, 20);
        }
      }
    }
  }*/

  // Click level
  if (mouse_b & 1 && collision (mouse_x, mouse_x, 320, 978, mouse_y, mouse_y, 220, 718)) {
    next_state = StateEngine::STATE_GAME;
  }
}
