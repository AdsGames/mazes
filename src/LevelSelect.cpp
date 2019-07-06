#include "LevelSelect.h"

#include <loadpng.h>
#include <fstream>

#include "globals.h"
#include "TransitionEffects.h"

LevelSelect::LevelSelect() {
  // Load images
  levelSelect = load_png ("images/levelSelect.png", nullptr);
  levelSelectLeft = load_png ("images/levelSelectLeft.png", nullptr);
  levelSelectRight = load_png ("images/levelSelectRight.png", nullptr);
  background = load_png ("images/background.png", nullptr);
  cursor[0] = load_png ("images/cursor1.png", nullptr);
  cursor[1] = load_png ("images/cursor2.png", nullptr);

  // Load sounds
  click = load_sample ("sfx/click.wav");

  // Create button
  back = Button(380, 40);

  // Sets button images
  back.SetImages ("images/buttons/back.png", "images/buttons/back_hover.png");
}

LevelSelect::~LevelSelect() {

}

//Collision
bool LevelSelect::collision (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2) {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}

void LevelSelect::draw (BITMAP *buffer) {
  draw_sprite (buffer, levelSelect, 0, 0);
  back.Draw (buffer);
  stretch_sprite (buffer, background, 320, 220, 640, 480);
  textprintf_centre_ex (buffer, font, 640, 760, makecol (0, 0, 0), makecol (255, 255, 255), "Level:%i", levelOn);

  //Click buttons
  if (collision (mouse_x, mouse_x, -1, 200, mouse_y, mouse_y, 0, 960)) {
    draw_sprite (buffer, levelSelectLeft, 0, 0);
    draw_sprite (buffer, cursor[1], mouse_x, mouse_y);
  }
  else if (collision (mouse_x, mouse_x, 1080, 1280, mouse_y, mouse_y, 0, 960)) {
    draw_sprite (buffer, levelSelectRight, 1080, 0);
    draw_sprite (buffer, cursor[1], mouse_x, mouse_y);
  }
  else if (collision (mouse_x, mouse_x, 320, 978, mouse_y, mouse_y, 220, 718)) {
    draw_sprite (buffer, cursor[1], mouse_x, mouse_y);
  }
  else {
    draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
  }
}

void LevelSelect::update (StateEngine &engine) {
  //Go to menu
  if (key[KEY_M] || joy[0].button[1].b) {
    TransitionEffects::highcolor_fade_out (16);
    engine.setNextState (StateEngine::STATE_MENU);
  }

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

  //Click buttons
  if (collision (mouse_x, mouse_x, -1, 200, mouse_y, mouse_y, 0, 960)) {
    if (mouse_b & 1 && levelOn > 1) {
      play_sample (click, 255, 125, 1000, 0);
      levelOn -= 1;
      //setupGame (false);
      rest (140);
    }
  }
  else if (collision (mouse_x, mouse_x, 1080, 1280, mouse_y, mouse_y, 0, 960)) {
    if (mouse_b & 1) {
      std::ifstream read ("levels/level" + std::to_string (levelOn + 1) + ".map");

      if (!read.fail()) {
        play_sample (click, 255, 125, 1000, 0);
        levelOn += 1;
        //setupGame (false);
        rest (140);
      }

      read.close();
    }
  }
  else if (collision (mouse_x, mouse_x, 320, 978, mouse_y, mouse_y, 220, 718)) {
    if (mouse_b & 1) {
      TransitionEffects::highcolor_fade_out (16);
      engine.setNextState (StateEngine::STATE_GAME);
      //changeMap();
    }
  }

  if (mouse_b & 1 && back.Hover() == true) {
    TransitionEffects::highcolor_fade_out (16);
    engine.setNextState (StateEngine::STATE_MENU);
  }
}
