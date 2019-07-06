#include "Game.h"

#include <loadpng.h>

#include "utility/tools.h"
#include "utility/AssetLoader.h"
#include "utility/TransitionEffects.h"

Game::Game() {
  won = 0;
  haveBroom = 0;
  x = 40;
  y = 40;
  lives = 3;
  characterRotation = 0;

  gameScreen = 0;
  currentLevel = 1;
  score = 0;
  exitGame = false;
  levelText = "";

  character[0] = AssetLoader::LoadPng ("images/blocks/3d/character_down.png");
  character[1] = AssetLoader::LoadPng ("images/blocks/3d/character_up.png");
  character[2] = AssetLoader::LoadPng ("images/blocks/3d/character_left.png");
  character[3] = AssetLoader::LoadPng ("images/blocks/3d/character_right.png");

  wall = AssetLoader::LoadPng ("images/blocks/3d/wall.png");
  wall2 = AssetLoader::LoadPng ("images/blocks/3d/wall2.png");
  robot = AssetLoader::LoadPng ("images/blocks/3d/robot.png");
  box = AssetLoader::LoadPng ("images/blocks/3d/box.png");
  scrap = AssetLoader::LoadPng ("images/blocks/3d/scrap.png");
  broom = AssetLoader::LoadPng ("images/blocks/3d/broom.png");
  garbagecan = AssetLoader::LoadPng ("images/blocks/3d/garbagecan.png");
  janitorroom = AssetLoader::LoadPng ("images/blocks/3d/janitor_room.png");
  janitorroomopen = AssetLoader::LoadPng ("images/blocks/3d/janitor_room_open.png");
  wood_floor = AssetLoader::LoadPng ("images/blocks/3d/wood_floor.png");
  background = AssetLoader::LoadPng ("images/background.png");


  // Sounds and music
  sweep = AssetLoader::LoadWav ("sfx/sweep.wav");
  explode = AssetLoader::LoadWav ("sfx/explode.wav");
  trash = AssetLoader::LoadWav ("sfx/trash.wav");
  oof = AssetLoader::LoadWav ("sfx/oof.wav");
  winsound = AssetLoader::LoadWav ("sfx/winsound.wav");
  door = AssetLoader::LoadWav ("sfx/door.wav");
  hitwall = AssetLoader::LoadWav ("sfx/hitwall.wav");
  boxhitwall = AssetLoader::LoadWav ("sfx/boxhitwall.wav");
  boxslide = AssetLoader::LoadWav ("sfx/boxslide.wav");

  song = AssetLoader::LoadOgg ("sfx/music.ogg");

  // Sets time
  startTime = clock();
  currentTime = clock();

  // Sets Font
  font = AssetLoader::LoadPcxFont ("fonts/arial_black.pcx");

  // Background Music
  play_sample (song, 255, 128, 1000, 1);
}

Game::~Game() {
  //dtor
}

void Game::draw (BITMAP *buffer) {
  //Draw background
  draw_sprite (buffer, background, 0, 0);

  //Draws Tiles
  for (int i = 0; i < 24; i++) {
    for (int t = 31; t >= 0; t--) {
      if (y / 40 == i && x / 40 == t) {
        //Draws Character
        if (perspective == 0) {
          //rotate_sprite(buffer, character, x + 5, y + 5, ftofix(characterRotation));
          if (characterRotation == 0) {
            draw_sprite (buffer, character[0], x, y);
          }
          else if (characterRotation == 128) {
            draw_sprite (buffer, character[1], x, y);
          }
          else if (characterRotation == 64) {
            draw_sprite (buffer, character[2], x, y);
          }
          else if (characterRotation == 192) {
            draw_sprite (buffer, character[3], x, y);
          }
        }
        else {
          draw_sprite (buffer, character[0], x, y);
        }

        //Draws broom if needed
        if ((key[KEY_SPACE] || joy[0].button[0].b) && haveBroom == 1) {
          if (perspective == 0) {
            rotate_sprite (buffer, broom, x + 10, y + 10, ftofix (float (characterRotation)));
          }
          else {
            draw_sprite (buffer, broom, x, y);
          }
        }
      }

      //Reset blocks
      if (tiles[i][t].value != map[i][t]) {
        resetBlocks (i, t);
      }

      map[i][t] = tiles[i][t].value;

      if (tiles[i][t].image[0] != NULL && tiles[i][t].value != 1) {
        if (perspective == 0) {
          stretch_sprite (buffer, tiles[i][t].image[0], t * 16 + 320 + i * 16, i * 8 - t * 8 + 220, 32, 64);
        }
        else if (perspective == 1) {
          stretch_sprite (buffer, tiles[i][t].image[0], t * 20 + 320, i * 20 + 220, 20, 20);
        }
      }
    }
  }

  //Draws Stats
  textprintf_ex (buffer, font, 0, 0, makecol (0, 0, 0), makecol (255, 255, 255), "Score:%i", score);
  textprintf_right_ex (buffer, font, 1280, 0, makecol (0, 0, 0), makecol (255, 255, 255), "Robots Left:%i", robotsLeft);
  textprintf_centre_ex (buffer, font, 640, 0, makecol (0, 0, 0), makecol (255, 255, 255), "Lives:%i", lives);
}

void Game::update (StateEngine &engine) {
  //Go to menu
  if (key[KEY_M] || joy[0].button[1].b) {
    TransitionEffects::FadeOut (16);
    engine.setNextState (StateEngine::STATE_MENU);
    score = 0;
  }

  //Changes move
  moving = false;

  //Update Clock
  currentTime = clock();

  //Checks if movther
  if (int (currentTime - startTime) / CLOCKS_PER_SEC > (elaspedTime)) {
    moving = true;
  }

  //Updates Elasped Time
  elaspedTime = int (currentTime - startTime) / CLOCKS_PER_SEC;



  //Updates robots positions
  if (step % 4 == 0) {
    for (int i = 0; i < 24; i++) {
      for (int t = 0; t < 32; t++) {
        if (tiles[i][t].value == 1) {
          int lowest = 0, highest = 5;
          int range = (highest - lowest) + 1;
          int random_integer = lowest + int (range * rand() / (RAND_MAX + 1.0));

          if (random_integer == 1 && tiles[i - 1][t].value == 0) {
            tiles[i - 1][t].value = 1;
            tiles[i][t].value = 0;
          }

          if (random_integer == 2 && tiles[i][t + 1].value == 0) {
            tiles[i][t + 1].value = 1;
            tiles[i][t].value = 0;
          }

          if (random_integer == 3 && tiles[i + 1][t].value == 0) {
            tiles[i + 1][t].value = 1;
            tiles[i][t].value = 0;
          }

          if (random_integer == 4 && tiles[i][t - 1].value == 0) {
            tiles[i][t - 1].value = 1;
            tiles[i][t].value = 0;
          }

          if (i == y / 40 && t == x / 40) {
            play_sample (oof, 2000, 122, 1000, 0);
            lives -= 1;
            x = 40;
            y = 40;
          }
        }
      }
    }
  }

  //Restart Map
  if (key[KEY_R] || joy[0].button[2].b) {
    TransitionEffects::FadeOut (16);
    changeMap();
  }

  //Pause Game
  if (key[KEY_P] || joy[0].button[7].b) {
    while (key[KEY_P] || joy[0].button[7].b) {
      poll_joystick();
    }

    /*while (! (key[KEY_P] || joy[0].button[7].b || key[KEY_ESC])) {
      textout_ex (buffer, font, "Paused press P to resume", 420, 440, makecol (0, 0, 0), makecol (255, 255, 255));
      draw_sprite (screen, buffer, 0, 0);
      poll_joystick();
    }*/

    while (key[KEY_P] || joy[0].button[7].b) {
      poll_joystick();
    }
  }

  //Finish Level
  if (doneLevel == 1) {
    play_sample (winsound, 255, 122, 1000, 0);
    currentLevel += 1;
    lives += 1;

    if (!changeMap()) {
      TransitionEffects::FadeOut (16);
      engine.setNextState (StateEngine::STATE_WIN);
    }
  }

  //Die
  if (lives <= 0) {
    changeMap();
    score = 0;
    //setupGame (false);
  }

  //Checks for character move
  charactermove();

  //Rests (regulates game speed)
  rest (100);
}

//Controls Character Movements
void Game::charactermove() {
  int delta_x = 0;
  int delta_y = 0;
  int map_x = x / 40;
  int map_y = y / 40;

  if (key[KEY_UP] || key[KEY_W] || joy[0].stick[0].axis[1].d1) {
    characterRotation = 128;
    delta_y = -1;
  }
  else if (key[KEY_DOWN] || key[KEY_S] || joy[0].stick[0].axis[1].d2) {
    characterRotation = 0;
    delta_y = 1;
  }
  else if (key[KEY_LEFT] || key[KEY_A] || joy[0].stick[0].axis[0].d1) {
    characterRotation = 64;
    delta_x = -1;
  }
  else if (key[KEY_RIGHT] || key[KEY_D] || joy[0].stick[0].axis[0].d2) {
    characterRotation = 192;
    delta_x = 1;
  }

  if (delta_x != 0 || delta_y != 0) {
    tile *tile_at_pos = &tiles[map_y + delta_y][map_x + delta_x];
    tile *tile_behind_pos = &tiles[map_y + delta_y * 2][map_x + delta_x * 2];

    if (tile_at_pos -> type == AIR) {
      y += delta_y * 40;
      x += delta_x * 40;
    }
    else if (tile_at_pos -> type == SOLID && tile_at_pos -> value != 7 && tile_at_pos -> value != 9 && tile_at_pos -> value != 3) {
      play_sample (hitwall, 255, 122, 1000, 0);
    }
    else if (tile_at_pos -> type == PUSHABLE) {
      if (tile_behind_pos -> type == AIR) {
        play_sample (boxslide, 255, 122, 1000, 0);
        tiles[y / 40 - 2][x / 40].value = tiles[y / 40 - 1][x / 40].value;
        tiles[y / 40 - 1][x / 40].value = 0;
        y += delta_y * 40;
        x += delta_x * 40;
      }
      else if (tile_behind_pos -> type == ENEMY) {
        play_sample (explode, 255, 122, 1000, 0);
        tiles[y / 40 - 2][x / 40].value = 3;
        score += 100;
      }
      else {
        play_sample (boxhitwall, 255, 122, 1000, 0);
      }
    }
    else if (key[KEY_SPACE] && tile_at_pos -> value == 3 && haveBroom) {
      if (tile_behind_pos -> type == AIR) {
        tile_behind_pos -> value = tile_at_pos -> value;
        tile_at_pos -> value = 0;
      }
      else if (tile_behind_pos -> value == 6) {
        score += 50;
        robotsLeft -= (robotsLeft > 0);
        play_sample (trash, 255, 122, 1000, 0);
        tile_at_pos -> value = 0;
      }

      play_sample (sweep, 1000, 122, 1000, 0);
    }
    else if (tiles[y / 40 - 1][x / 40].value == 7) {
      if (!haveBroom) {
        tile_at_pos -> value = 9;
        play_sample (door, 255, 122, 1000, 0);
        haveBroom = true;
      }
    }
    else if (tile_at_pos -> value == 9) {
      doneLevel = (robotsLeft == 0);
    }
  }

  mousePressed = false;
}

// Update Blocks
void Game::resetBlocks (int newI, int newT) {
  // Assign images to data values
  for (int i = 0; i < 24; i++) {
    for (int t = 0; t < 32; t++) {
      if (newI != -1 && newT != -1) {
        i = newI;
        t = newT;
      }

      if (tiles[i][t].value == 0) {
        tiles[i][t].image[0] = NULL;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = AIR;
      }
      else if (tiles[i][t].value == 1) {
        tiles[i][t].image[0] = robot;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = ENEMY;
      }
      else if (tiles[i][t].value == 2) {
        tiles[i][t].image[0] = box;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = PUSHABLE;
      }
      else if (tiles[i][t].value == 3) {
        tiles[i][t].image[0] = scrap;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else if (tiles[i][t].value == 4) {
        tiles[i][t].image[0] = NULL;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = AIR;
      }
      else if (tiles[i][t].value == 5) {
        tiles[i][t].image[0] = wall;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else if (tiles[i][t].value == 6) {
        tiles[i][t].image[0] = garbagecan;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else if (tiles[i][t].value == 7) {
        tiles[i][t].image[0] = janitorroom;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else if (tiles[i][t].value == 8) {
        tiles[i][t].image[0] = wall2;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else if (tiles[i][t].value == 9) {
        tiles[i][t].image[0] = janitorroomopen;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = SOLID;
      }
      else {
        tiles[i][t].image[0] = NULL;
        tiles[i][t].image[1] = NULL;
        tiles[i][t].type = AIR;
      }

      if (newI != -1 && newT != -1) {
        break;
      }
    }

    if (newI != -1 && newT != -1) {
      break;
    }
  }
}

//Change tiles
bool Game::changeMap() {
  finalFile = "levels/level" + std::to_string (currentLevel) + ".map";

  doneLevel = 0;
  haveBroom = 0;
  x = 40;
  y = 40;

  std::ifstream read (finalFile.c_str());

  if (!read.fail()) {
    std::string token = "";
    std::string tag = "";
    levelText = "";
    robotsLeft = 0;
    std::regex tag_pattern ("\\[.*\\]");

    int i = 0;
    int t = 0;

    while (!read.eof()) {
      // Get current tag
      read >> token;

      if (std::regex_match (token, tag_pattern)) {
        tag = token;
        read >> token;
      }

      // Get title
      if (!tag.compare ("[title]")) {
        levelText += token + " ";
      }

      // Get #robots
      if (!tag.compare ("[robots]")) {
        robotsLeft = std::stoi (token);
      }

      // Get map
      if (!tag.compare ("[map]")) {
        map[i][t] = std::stoi (token);
        std::cout << tag << " = " <<   i << std::endl;
        i++;

        if (i >= 24) {
          t++;
          i = 0;
        }
      }
    }

    resetBlocks (-1, -1);

    if (gameScreen == 3) {
      textprintf_centre_ex (screen, font, 640, 460, makecol (255, 255, 255), -2, "%s", levelText.c_str());
      rest (1000);
      TransitionEffects::FadeOut (16);
    }

    read.close();
    return true;
  }

  read.close();
  return false;
}
