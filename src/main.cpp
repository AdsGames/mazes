/*
 *  Allan Legemaate
 *  2012
 *  Mazes Main
 */

#include <allegro.h>
#include <loadpng.h>
#include <logg.h>

#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "button.h"
#include "id.h"

//Creates Buttons
Button start;
Button help;
Button quit;
Button mode;
Button back;

// Menu
int selectorY, selectorX, newSelectorY, selected_object;
int cursor_x, cursor_y;
int menu_view_x, menu_view_y;

//Creates integers
int lives;
int currentLevel;
int robotsLeft;
int x;
int y;
int score;
int perspective;
int step;
int characterRotation;
float elaspedTime;
bool haveBroom;
bool won;
bool doneLevel;
bool mousePressed;
bool moving;
int gameScreen;
bool exitGame;

std::string levelText;

//Used for file loading
std::string finalFile;

//Main Map
int map[24][32] = {0};

//Define bitmaps
BITMAP *buffer;
BITMAP *character[4];
BITMAP *background;
BITMAP *robot;
BITMAP *wall;
BITMAP *fakeWall;
BITMAP *ground;
BITMAP *box;
BITMAP *menu;
BITMAP *help_menu;
BITMAP *cursor[2];
BITMAP *levelSelect;
BITMAP *levelSelectLeft;
BITMAP *levelSelectRight;
BITMAP *winscreen;
BITMAP *intro;
BITMAP *splash;

BITMAP *scrap;
BITMAP *broom;
BITMAP *garbagecan;
BITMAP *janitorroom;
BITMAP *janitorroomopen;
BITMAP *wall2;
BITMAP *wood_floor;

//Define sound effects
SAMPLE *song;
SAMPLE *hitwall;
SAMPLE *boxhitwall;
SAMPLE *boxslide;
SAMPLE *explode;
SAMPLE *sweep;
SAMPLE *trash;
SAMPLE *oof;
SAMPLE *winsound;
SAMPLE *door;
SAMPLE *click;

void restart();
void update();
void restartlevel();
void loadLevel (char fileName[]);
void changeMap();

//Timers
clock_t startTime;
clock_t currentTime;

//Tiles on screen
struct tile {
  BITMAP *image[2];
  int value;
  int type;
  int dir;
};

tile tiles[24][32];

//Collision
bool collision (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2) {
  return (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1);
}

void highcolor_fade_in (BITMAP *bmp_orig, int speed) {
  BITMAP *bmp_buff;

  if ((bmp_buff = create_bitmap (SCREEN_W, SCREEN_H))) {
    int a;

    if (speed <= 0)
      speed = 16;

    for (a = 0; a < 256; a += speed) {
      clear (bmp_buff);
      set_trans_blender (0, 0, 0, a);
      draw_trans_sprite (bmp_buff, bmp_orig, 0, 0);
      vsync();
      blit (bmp_buff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
    }

    destroy_bitmap (bmp_buff);
  }

  blit (bmp_orig, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
}

void highcolor_fade_out (int speed) {
  BITMAP *bmp_orig;

  if ((bmp_orig = create_bitmap (SCREEN_W, SCREEN_H))) {
    BITMAP *bmp_buff;

    if ((bmp_buff = create_bitmap (SCREEN_W, SCREEN_H))) {
      int a;
      blit (screen, bmp_orig, 0, 0, 0, 0, SCREEN_W, SCREEN_H);

      if (speed <= 0)
        speed = 16;

      for (a = 255 - speed; a > 0; a -= speed) {
        clear (bmp_buff);
        set_trans_blender (0, 0, 0, a);
        draw_trans_sprite (bmp_buff, bmp_orig, 0, 0);
        vsync();
        blit (bmp_buff, screen, 0, 0, 0, 0, SCREEN_W, SCREEN_H);
      }

      destroy_bitmap (bmp_buff);
    }

    destroy_bitmap (bmp_orig);
  }

  rectfill (screen, 0, 0, SCREEN_W, SCREEN_H, makecol (0, 0, 0));
}

void setupGame (bool first) {
  won = 0;
  haveBroom = 0;
  x = 40;
  y = 40;
  lives = 3;
  characterRotation = 0;

  if (first) {
    gameScreen = 0;
    currentLevel = 1;
    score = 0;
    buffer = create_bitmap (1280, 960);
    exitGame = false;
    levelText = "";

    if (perspective == 0) {
      character[0] = load_png ("images/blocks/3d/character_down.png", nullptr);
      character[1] = load_png ("images/blocks/3d/character_up.png", nullptr);
      character[2] = load_png ("images/blocks/3d/character_left.png", nullptr);
      character[3] = load_png ("images/blocks/3d/character_right.png", nullptr);

      wall = load_png ("images/blocks/3d/wall.png", nullptr);
      wall2 = load_png ("images/blocks/3d/wall2.png", nullptr);
      robot = load_png ("images/blocks/3d/robot.png", nullptr);
      box = load_png ("images/blocks/3d/box.png", nullptr);
      scrap = load_png ("images/blocks/3d/scrap.png", nullptr);
      broom = load_png ("images/blocks/3d/broom.png", nullptr);
      garbagecan = load_png ("images/blocks/3d/garbagecan.png", nullptr);
      janitorroom = load_png ("images/blocks/3d/janitor_room.png", nullptr);
      janitorroomopen = load_png ("images/blocks/3d/janitor_room_open.png", nullptr);
      wood_floor = load_png ("images/blocks/3d/wood_floor.png", nullptr);
    }

    if (perspective == 1) {
      character[0] = load_png ("images/blocks/2d/character.png", nullptr);
      character[1] = load_png ("images/blocks/2d/character.png", nullptr);
      character[2] = load_png ("images/blocks/2d/character.png", nullptr);
      character[3] = load_png ("images/blocks/2d/character.png", nullptr);

      wall = load_png ("images/blocks/2d/wall.png", nullptr);
      wall2 = load_png ("images/blocks/2d/wall2.png", nullptr);
      robot = load_png ("images/blocks/2d/robot.png", nullptr);
      box = load_png ("images/blocks/2d/box.png", nullptr);
      scrap = load_png ("images/blocks/2d/scrap.png", nullptr);
      broom = load_png ("images/blocks/2d/broom.png", nullptr);
      garbagecan = load_png ("images/blocks/2d/garbagecan.png", nullptr);
      janitorroom = load_png ("images/blocks/2d/janitor_room.png", nullptr);
      janitorroomopen = load_png ("images/blocks/2d/janitor_room_open.png", nullptr);
      wood_floor = load_png ("images/blocks/2d/wood_floor.png", nullptr);
    }

    menu = load_png ("images/menu.png", nullptr);
    background = load_png ("images/background.png", nullptr);
    help_menu = load_png ("images/help.png", nullptr);
    intro = load_png ("images/intro.png", nullptr);
    splash = load_png ("images/splash.png", nullptr);
    winscreen = load_png ("images/winscreen.png", nullptr);
    levelSelect = load_png ("images/levelSelect.png", nullptr);
    levelSelectLeft = load_png ("images/levelSelectLeft.png", nullptr);
    levelSelectRight = load_png ("images/levelSelectRight.png", nullptr);

    // Sounds and music
    sweep = load_sample ("sfx/sweep.wav");
    explode = load_sample ("sfx/explode.wav");
    trash = load_sample ("sfx/trash.wav");
    oof = load_sample ("sfx/oof.wav");
    winsound = load_sample ("sfx/winsound.wav");
    door = load_sample ("sfx/door.wav");
    hitwall = load_sample ("sfx/hitwall.wav");
    boxhitwall = load_sample ("sfx/boxhitwall.wav");
    boxslide = load_sample ("sfx/boxslide.wav");
    click = load_sample ("sfx/click.wav");

    song = logg_load ("sfx/music.ogg");

    srand (time (nullptr));

    // Sets button images
    start.SetImages ("images/buttons/start.png", "images/buttons/start_hover.png");
    help.SetImages ("images/buttons/help.png", "images/buttons/help_hover.png");
    quit.SetImages ("images/buttons/quit.png", "images/buttons/quit_hover.png");
    mode.SetImages ("images/buttons/mode_3d.png", "images/buttons/mode_3d_hover.png");
    back.SetImages ("images/buttons/back.png", "images/buttons/back_hover.png");

    // Sets button positions
    start.SetX (380);
    help.SetX (380);
    quit.SetX (380);
    mode.SetX (380);
    back.SetX (380);

    start.SetY (240);
    help.SetY (380);
    quit.SetY (520);
    mode.SetY (660);
    back.SetY (40);

    // Sets Cursors
    cursor[0] = load_png ("images/cursor1.png", nullptr);
    cursor[1] = load_png ("images/cursor2.png", nullptr);

    // Sets time
    startTime = clock();
    currentTime = clock();

    // Sets Font
    font = load_font ("fonts/arial_black.pcx", nullptr, nullptr);
  }

  // Background Music
  //play_sample (song, 255, 128, 1000, 1);
}


// Update Blocks
void resetBlocks (int newI, int newT) {
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

//Controls Character Movements
void charactermove() {
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

//Change tiles
void changeMap() {
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
      highcolor_fade_out (16);
    }
  }
  else if (gameScreen == 3) {
    highcolor_fade_out (16);
    gameScreen = 4;
    highcolor_fade_in (winscreen, 16);
  }

  read.close();
}

//Updates games
void game() {
  setupGame (true);

  while (!exitGame) {
    // Controller Support
    poll_joystick();

    if (key[KEY_ESC] || joy[0].button[3].b) {
      exitGame = true;
    }

    //Splash
    if (gameScreen == 0) {
      /* highcolor_fade_in (intro, 16);
       rest (1000);
       highcolor_fade_out (16);
       highcolor_fade_in (splash, 16);
       rest (1000);
       highcolor_fade_out (16);*/
      gameScreen = 1;
      //highcolor_fade_in (menu, 16);
    }

    //Menu
    else if (gameScreen == 1) {
      //Checks for mouse press
      if (mouse_b & 1) {
        if (start.Hover()) {
          highcolor_fade_out (16);
          gameScreen = 2;
          setupGame (false);
          highcolor_fade_in (levelSelect, 16);
        }
        else if (help.Hover()) {
          highcolor_fade_in (help_menu, 16);

          do {
            draw_sprite (buffer, help_menu, 0, 0);
            draw_sprite (screen, buffer, 0, 0);
          } while (! (key[KEY_SPACE] || key[KEY_ENTER] || mouse_b & 1 || joy[0].button[1].b || joy[0].button[0].b));

          highcolor_fade_out (16);
        }
        else if (quit.Hover()) {
          exitGame = true;
        }
        else if (mode.Hover()) {
          if (perspective == 0) {
            perspective = 1;
            setupGame (true);
            mode.SetImages ("images/buttons/mode_2d.png", "images/buttons/mode_2d_hover.png");
            mouse_b = false;
          }
          else if (perspective == 1) {
            perspective = 0;
            setupGame (true);
            mode.SetImages ("images/buttons/mode_3d.png", "images/buttons/mode_3d_hover.png");
            mouse_b = false;
          }
        }
      }

      //Draws Menu
      draw_sprite (buffer, menu, 0, 0);

      //Draws Buttons
      start.draw (buffer);
      help.draw (buffer);
      quit.draw (buffer);
      mode.draw (buffer);

      //Draws Cursor
      draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
    }

    //Level Select
    if (gameScreen == 2) {
      // Die
      if (tiles[y / 40][x / 40].value == 1) {
        lives -= 1;
        x = 40;
        y = 40;
        play_sample (oof, 255, 122, 1000, 0);
      }

      //Go to menu
      if (key[KEY_M] || joy[0].button[1].b) {
        highcolor_fade_out (16);
        gameScreen = 1;
        highcolor_fade_in (menu, 16);
      }

      draw_sprite (buffer, levelSelect, 0, 0);
      back.draw (buffer);
      stretch_sprite (buffer, background, 320, 220, 640, 480);
      textprintf_centre_ex (buffer, font, 640, 760, makecol (0, 0, 0), makecol (255, 255, 255), "Level:%i", currentLevel);

      //Mini tiles tiles
      for (int i = 0; i < 24; i++) {
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
      }

      //Click buttons
      if (collision (mouse_x, mouse_x, -1, 200, mouse_y, mouse_y, 0, 960)) {
        draw_sprite (buffer, levelSelectLeft, 0, 0);
        draw_sprite (buffer, cursor[1], mouse_x, mouse_y);

        if (mouse_b & 1 && currentLevel > 1) {
          play_sample (click, 255, 125, 1000, 0);
          currentLevel -= 1;
          setupGame (false);
          rest (140);
        }
      }
      else if (collision (mouse_x, mouse_x, 1080, 1280, mouse_y, mouse_y, 0, 960)) {
        draw_sprite (buffer, levelSelectRight, 1080, 0);
        draw_sprite (buffer, cursor[1], mouse_x, mouse_y);

        if (mouse_b & 1) {
          finalFile = "levels/level" + std::to_string (currentLevel + 1) + ".map";
          std::ifstream read (finalFile.c_str());

          if (!read.fail()) {
            play_sample (click, 255, 125, 1000, 0);
            currentLevel += 1;
            setupGame (false);
            rest (140);
          }

          read.close();
        }
      }
      else if (collision (mouse_x, mouse_x, 320, 978, mouse_y, mouse_y, 220, 718)) {
        draw_sprite (buffer, cursor[1], mouse_x, mouse_y);

        if (mouse_b & 1) {
          highcolor_fade_out (16);
          gameScreen = 3;
          changeMap();
          setupGame (false);
        }
      }
      else {
        draw_sprite (buffer, cursor[0], mouse_x, mouse_y);
      }

      if (mouse_b & 1 && back.Hover() == true) {
        highcolor_fade_out (16);
        gameScreen = 1;
        highcolor_fade_in (menu, 16);
      }
    }

    //Ingame Loops
    if (gameScreen == 3) {
      //Go to menu
      if (key[KEY_M] || joy[0].button[1].b) {
        highcolor_fade_out (16);
        gameScreen = 1;
        score = 0;
        highcolor_fade_in (menu, 16);
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

      //Draw background
      draw_sprite (buffer, background, 0, 0);

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
        highcolor_fade_out (16);
        changeMap();
      }

      //Pause Game
      if (key[KEY_P] || joy[0].button[7].b) {
        while (key[KEY_P] || joy[0].button[7].b) {
          poll_joystick();
        }

        while (! (key[KEY_P] || joy[0].button[7].b || key[KEY_ESC])) {
          textout_ex (buffer, font, "Paused press P to resume", 420, 440, makecol (0, 0, 0), makecol (255, 255, 255));
          draw_sprite (screen, buffer, 0, 0);
          poll_joystick();
        }

        while (key[KEY_P] || joy[0].button[7].b) {
          poll_joystick();
        }
      }

      //Finish Level
      if (doneLevel == 1) {
        play_sample (winsound, 255, 122, 1000, 0);
        currentLevel += 1;
        lives += 1;
        changeMap();
      }

      //Die
      if (lives <= 0) {
        changeMap();
        score = 0;
        setupGame (false);
      }

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

      //Draws Cursor
      draw_sprite (buffer, cursor[0], (mouse_x / 40) * 40, (mouse_y / 40) * 40);

      //Checks for character move
      charactermove();

      //Rests (regulates game speed)
      rest (100);
    }
    else if (gameScreen == 4) {
      draw_sprite (buffer, winscreen, 0, 0);
      textprintf_ex (buffer, font, 310, 400, makecol (0, 0, 0), -1, "%i", score);

      if (key[KEY_SPACE] || key[KEY_ENTER] || mouse_b & 1  || joy[0].button[0].b) {
        highcolor_fade_out (16);
        gameScreen = 1;
        highcolor_fade_in (winscreen, 16);
      }
    }

    //Adds one to step
    step++;

    //Draws Buffer
    draw_sprite (screen, buffer, 0, 0);
  }
}

void close() {
  for (int i = 0; i < 4; i++) {
    if (character[i]) {
      destroy_bitmap (character[i]);
    }
  }

  for (int i = 0; i < 2; i++) {
    if (cursor[i]) {
      destroy_bitmap (cursor[i]);
    }
  }

  destroy_bitmap (buffer);
  destroy_bitmap (background);
  destroy_bitmap (robot);
  destroy_bitmap (wall);
  destroy_bitmap (fakeWall);
  destroy_bitmap (ground);
  destroy_bitmap (box);
  destroy_bitmap (menu);
  destroy_bitmap (help_menu);
  destroy_bitmap (levelSelect);
  destroy_bitmap (levelSelectLeft);
  destroy_bitmap (levelSelectRight);
  destroy_bitmap (winscreen);
  destroy_bitmap (intro);
  destroy_bitmap (splash);
  destroy_bitmap (scrap);
  destroy_bitmap (broom);
  destroy_bitmap (garbagecan);
  destroy_bitmap (janitorroom);
  destroy_bitmap (janitorroomopen);
  destroy_bitmap (wall2);
  destroy_bitmap (wood_floor);

  destroy_sample (hitwall);
  destroy_sample (boxhitwall);
  destroy_sample (boxslide);
  destroy_sample (explode);
  destroy_sample (sweep);
  destroy_sample (trash);
  destroy_sample (oof);
  destroy_sample (winsound);
  destroy_sample (door);
  destroy_sample (click);

  // Allegro
  clear_keybuf();
}

//Main functions run here
int main() {
  //Initializing
  allegro_init();
  install_timer();
  install_mouse();
  install_joystick (JOY_TYPE_AUTODETECT);
  install_keyboard();
  set_color_depth (32);
  set_gfx_mode (GFX_AUTODETECT_WINDOWED, 1280, 960, 0, 0);
  install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, ".");
  set_window_title ("Mazes");

  //Starts Game
  game();

  close();

  rest (100);

  return 0;
}

END_OF_MAIN()
