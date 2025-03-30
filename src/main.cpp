/*
 *  Allan Legemaate
 *  2012
 *  Mazes Main
 */

#include <asw/asw.h>
#include <array>
#include <cmath>
#include <ctime>
#include <fstream>
#include <sstream>
#include <string>

#include "button.h"
#include "id.h"

// Creates Buttons
Button start;
Button help;
Button quit;
Button mode;
Button back;

// Fonts
asw::Font font;
bool helpOpen = false;

// Creates integers
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

std::string levelText;

// Used for file loading
std::string finalFile;

// Main Map
int map[24][32] = {{5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0,
                    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5},
                   {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
                    5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}};

// Define bitmaps
std::array<asw::Texture, 4> character;
asw::Texture background;
asw::Texture robot;
asw::Texture wall;
asw::Texture fakeWall;
asw::Texture ground;
asw::Texture box;
asw::Texture menu;
asw::Texture help_menu;
asw::Texture levelSelect;
asw::Texture levelSelectLeft;
asw::Texture levelSelectRight;
asw::Texture winscreen;
asw::Texture intro;
asw::Texture splash;

asw::Texture scrap;
asw::Texture broom;
asw::Texture garbagecan;
asw::Texture janitorroom;
asw::Texture janitorroomopen;
asw::Texture wall2;
asw::Texture wood_floor;

// Define sound effects
asw::Music song;
asw::Sample hitwall;
asw::Sample boxhitwall;
asw::Sample boxslide;
asw::Sample explode;
asw::Sample sweep;
asw::Sample trash;
asw::Sample oof;
asw::Sample winsound;
asw::Sample door;
asw::Sample click;

void changeMap();

// Timers
clock_t startTime;
clock_t currentTime;

// Tiles on screen
struct tile {
  std::array<asw::Texture, 2> image;
  int value;
  int type;
  int dir;
} tiles[24][32];

// Convert int to string
std::string convertInt(int number) {
  std::stringstream ss;
  ss << number;
  return ss.str();
}

// Checks if file exists
bool fexists(const char* filename) {
  std::ifstream ifile(filename);
  return !ifile.fail();
}

// Collision
bool collision(int xMin1,
               int xMax1,
               int xMin2,
               int xMax2,
               int yMin1,
               int yMax1,
               int yMin2,
               int yMax2) {
  if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1) {
    return true;
  }

  return false;
}

void rest(int ms) {
  clock_t endTime = clock() + ms;
  while (clock() < endTime) {
  }
}

void setupGame() {
  won = 0;
  haveBroom = 0;
  x = 40;
  y = 40;
  lives = 3;
  characterRotation = 0;
}

void initGame() {
  setupGame();

  gameScreen = 0;
  currentLevel = 1;
  score = 0;
  levelText = "";

  if (perspective == 0) {
    character[0] =
        asw::assets::loadTexture("assets/images/blocks/3d/character_down.png");
    character[1] =
        asw::assets::loadTexture("assets/images/blocks/3d/character_up.png");
    character[2] =
        asw::assets::loadTexture("assets/images/blocks/3d/character_left.png");
    character[3] =
        asw::assets::loadTexture("assets/images/blocks/3d/character_right.png");

    wall = asw::assets::loadTexture("assets/images/blocks/3d/wall.png");
    wall2 = asw::assets::loadTexture("assets/images/blocks/3d/wall2.png");
    robot = asw::assets::loadTexture("assets/images/blocks/3d/robot.png");
    box = asw::assets::loadTexture("assets/images/blocks/3d/box.png");
    scrap = asw::assets::loadTexture("assets/images/blocks/3d/scrap.png");
    broom = asw::assets::loadTexture("assets/images/blocks/3d/broom.png");
    garbagecan =
        asw::assets::loadTexture("assets/images/blocks/3d/garbagecan.png");
    janitorroom =
        asw::assets::loadTexture("assets/images/blocks/3d/janitor_room.png");
    janitorroomopen = asw::assets::loadTexture(
        "assets/images/blocks/3d/janitor_room_open.png");
    wood_floor =
        asw::assets::loadTexture("assets/images/blocks/3d/wood_floor.png");
  }

  if (perspective == 1) {
    character[0] =
        asw::assets::loadTexture("assets/images/blocks/2d/character.png");
    character[1] =
        asw::assets::loadTexture("assets/images/blocks/2d/character.png");
    character[2] =
        asw::assets::loadTexture("assets/images/blocks/2d/character.png");
    character[3] =
        asw::assets::loadTexture("assets/images/blocks/2d/character.png");

    wall = asw::assets::loadTexture("assets/images/blocks/2d/wall.png");
    wall2 = asw::assets::loadTexture("assets/images/blocks/2d/wall2.png");
    robot = asw::assets::loadTexture("assets/images/blocks/2d/robot.png");
    box = asw::assets::loadTexture("assets/images/blocks/2d/box.png");
    scrap = asw::assets::loadTexture("assets/images/blocks/2d/scrap.png");
    broom = asw::assets::loadTexture("assets/images/blocks/2d/broom.png");
    garbagecan =
        asw::assets::loadTexture("assets/images/blocks/2d/garbagecan.png");
    janitorroom =
        asw::assets::loadTexture("assets/images/blocks/2d/janitor_room.png");
    janitorroomopen = asw::assets::loadTexture(
        "assets/images/blocks/2d/janitor_room_open.png");
    wood_floor =
        asw::assets::loadTexture("assets/images/blocks/2d/wood_floor.png");
  }

  menu = asw::assets::loadTexture("assets/images/menu.png");
  background = asw::assets::loadTexture("assets/images/background.png");
  help_menu = asw::assets::loadTexture("assets/images/help.png");
  intro = asw::assets::loadTexture("assets/images/intro.png");
  splash = asw::assets::loadTexture("assets/images/splash.png");
  winscreen = asw::assets::loadTexture("assets/images/winscreen.png");
  levelSelect = asw::assets::loadTexture("assets/images/levelSelect.png");
  levelSelectLeft =
      asw::assets::loadTexture("assets/images/levelSelectLeft.png");
  levelSelectRight =
      asw::assets::loadTexture("assets/images/levelSelectRight.png");

  // Sounds and music
  sweep = asw::assets::loadSample("assets/sfx/sweep.wav");
  explode = asw::assets::loadSample("assets/sfx/explode.wav");
  trash = asw::assets::loadSample("assets/sfx/trash.wav");
  oof = asw::assets::loadSample("assets/sfx/oof.wav");
  winsound = asw::assets::loadSample("assets/sfx/winsound.wav");
  door = asw::assets::loadSample("assets/sfx/door.wav");
  hitwall = asw::assets::loadSample("assets/sfx/hitwall.wav");
  boxhitwall = asw::assets::loadSample("assets/sfx/boxhitwall.wav");
  boxslide = asw::assets::loadSample("assets/sfx/boxslide.wav");
  click = asw::assets::loadSample("assets/sfx/click.wav");

  song = asw::assets::loadMusic("assets/sfx/music.ogg");

  // Sets button images
  start.SetImages("assets/images/buttons/start.png",
                  "assets/images/buttons/start_hover.png");
  help.SetImages("assets/images/buttons/help.png",
                 "assets/images/buttons/help_hover.png");
  quit.SetImages("assets/images/buttons/quit.png",
                 "assets/images/buttons/quit_hover.png");
  mode.SetImages("assets/images/buttons/mode_3d.png",
                 "assets/images/buttons/mode_3d_hover.png");
  back.SetImages("assets/images/buttons/back.png",
                 "assets/images/buttons/back_hover.png");

  // Sets button positions
  start.SetX(380);
  help.SetX(380);
  quit.SetX(380);
  mode.SetX(380);
  back.SetX(380);

  start.SetY(240);
  help.SetY(380);
  quit.SetY(520);
  mode.SetY(660);
  back.SetY(40);

  // Sets time
  startTime = clock();
  currentTime = clock();

  // Sets Font
  font = asw::assets::loadFont("assets/fonts/dosis.ttf", 16);

  // Background Music
  asw::sound::playMusic(song, 255);
}

// Update Blocks
void resetBlocks(int newI, int newT) {
  // Assign images to data values
  for (int i = 0; i < 24; i++) {
    for (int t = 0; t < 32; t++) {
      if (newI != -1 && newT != -1) {
        i = newI;
        t = newT;
      }

      if (tiles[i][t].value == V_EMPTY) {
        tiles[i][t].image[0] = nullptr;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = AIR;
      } else if (tiles[i][t].value == V_ROBOT) {
        tiles[i][t].image[0] = robot;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = ENEMY;
      } else if (tiles[i][t].value == V_BOX) {
        tiles[i][t].image[0] = box;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = PUSHABLE;
      } else if (tiles[i][t].value == V_SCRAP) {
        tiles[i][t].image[0] = scrap;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else if (tiles[i][t].value == V_GROUND) {
        tiles[i][t].image[0] = nullptr;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = AIR;
      } else if (tiles[i][t].value == V_WALL) {
        tiles[i][t].image[0] = wall;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else if (tiles[i][t].value == V_GARBAGECAN) {
        tiles[i][t].image[0] = garbagecan;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else if (tiles[i][t].value == V_JANITORROOM) {
        tiles[i][t].image[0] = janitorroom;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else if (tiles[i][t].value == V_WALL2) {
        tiles[i][t].image[0] = wall2;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else if (tiles[i][t].value == V_JANITORROOMOPEN) {
        tiles[i][t].image[0] = janitorroomopen;
        tiles[i][t].image[1] = nullptr;
        tiles[i][t].type = SOLID;
      } else {
        tiles[i][t].image[0] = nullptr;
        tiles[i][t].image[1] = nullptr;
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

// Controls Character Movements
void characterMove() {
  if (asw::input::wasKeyPressed(asw::input::Key::UP) ||
      asw::input::wasKeyPressed(asw::input::Key::W)) {
    characterRotation = 128;

    if (tiles[y / 40 - 1][x / 40].type == AIR) {
      y -= 40;
    } else if (tiles[y / 40 - 1][x / 40].type == SOLID &&
               tiles[y / 40 - 1][x / 40].value != 7 &&
               tiles[y / 40 - 1][x / 40].value != 9 &&
               tiles[y / 40 - 1][x / 40].value != 3) {
      asw::sound::play(hitwall, 255, 122, 0);
    } else if (tiles[y / 40 - 1][x / 40].type == PUSHABLE &&
               tiles[y / 40 - 2][x / 40].type == AIR) {
      asw::sound::play(boxslide, 255, 122, 0);
      tiles[y / 40 - 2][x / 40].value = tiles[y / 40 - 1][x / 40].value;
      tiles[y / 40 - 1][x / 40].value = V_EMPTY;
      y -= 40;
    } else if (tiles[y / 40 - 1][x / 40].type == PUSHABLE &&
               tiles[y / 40 - 2][x / 40].type != AIR) {
      if (tiles[y / 40 - 2][x / 40].type == ENEMY) {
        asw::sound::play(explode, 255, 122, 0);
        tiles[y / 40 - 2][x / 40].value = V_SCRAP;
        score += 100;
      } else {
        asw::sound::play(boxhitwall, 255, 122, 0);
      }
    } else if (asw::input::isKeyDown(asw::input::Key::SPACE) &&
               tiles[y / 40 - 1][x / 40].value == V_SCRAP && haveBroom) {
      if (tiles[y / 40 - 2][x / 40].type == AIR) {
        tiles[y / 40 - 2][x / 40].value = tiles[y / 40 - 1][x / 40].value;
        tiles[y / 40 - 1][x / 40].value = V_EMPTY;
      } else if (tiles[y / 40 - 2][x / 40].value == V_GARBAGECAN) {
        score += 50;

        if (robotsLeft > 0) {
          robotsLeft -= 1;
        }

        asw::sound::play(trash, 255, 122, 0);
        tiles[y / 40 - 1][x / 40].value = V_EMPTY;
      }

      asw::sound::play(sweep, 1000, 122, 0);
    } else if (tiles[y / 40 - 1][x / 40].value == V_JANITORROOM) {
      if (!haveBroom) {
        tiles[y / 40 - 1][x / 40].value = V_JANITORROOMOPEN;
        asw::sound::play(door, 255, 122, 0);
        haveBroom = true;
      }
    } else if (tiles[y / 40 - 1][x / 40].value == V_JANITORROOMOPEN) {
      if (robotsLeft == 0) {
        doneLevel = 1;
      }
    }
  }

  if (asw::input::wasKeyPressed(asw::input::Key::DOWN) ||
      asw::input::wasKeyPressed(asw::input::Key::S)) {
    characterRotation = 0;

    if (tiles[y / 40 + 1][x / 40].value == V_EMPTY ||
        tiles[y / 40 + 1][x / 40].value == V_WALL2) {
      y = y + 40;
    } else if (tiles[y / 40 + 1][x / 40].value == V_WALL) {
      asw::sound::play(hitwall, 255, 122, 0);
    } else if (tiles[y / 40 + 1][x / 40].value == V_BOX &&
               tiles[y / 40 + 2][x / 40].value == V_EMPTY) {
      asw::sound::play(boxslide, 255, 122, 0);
      tiles[y / 40 + 1][x / 40].value = V_EMPTY;
      tiles[y / 40 + 2][x / 40].value = V_BOX;
      y = y + 40;
    } else if (tiles[y / 40 + 1][x / 40].value == V_BOX &&
               tiles[y / 40 + 2][x / 40].value != 0) {
      if (tiles[y / 40 + 1][x / 40].value == V_BOX &&
          tiles[y / 40 + 2][x / 40].value == V_ROBOT) {
        asw::sound::play(explode, 255, 122, 0);
        tiles[y / 40 + 2][x / 40].value = V_SCRAP;
        score += 100;
      } else {
        asw::sound::play(boxhitwall, 255, 122, 0);
      }
    } else if (tiles[y / 40 + 1][x / 40].value == V_SCRAP &&
               tiles[y / 40 + 2][x / 40].value == V_EMPTY && haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40 + 1][x / 40].value = V_EMPTY;
      tiles[y / 40 + 2][x / 40].value = V_SCRAP;
      asw::sound::play(sweep, 1000, 122, 0);
    } else if (tiles[y / 40 + 1][x / 40].value == V_SCRAP &&
               tiles[y / 40 + 2][x / 40].value == V_GARBAGECAN &&
               haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40 + 1][x / 40].value = V_EMPTY;
      score += 50;

      if (robotsLeft > 0) {
        robotsLeft -= 1;
      }

      asw::sound::play(sweep, 1000, 122, 0);
      asw::sound::play(trash, 255, 122, 0);
    } else if (tiles[y / 40 + 1][x / 40].value == V_JANITORROOM) {
      if (!haveBroom) {
        tiles[y / 40 + 1][x / 40].value = V_JANITORROOMOPEN;
        asw::sound::play(door, 255, 122, 0);
        haveBroom = true;
      }
    } else if (tiles[y / 40 + 1][x / 40].value == V_JANITORROOMOPEN) {
      if (robotsLeft == 0) {
        doneLevel = 1;
      }
    }
  }

  if (asw::input::wasKeyPressed(asw::input::Key::LEFT) ||
      asw::input::wasKeyPressed(asw::input::Key::A)) {
    characterRotation = 64;

    if (tiles[y / 40][x / 40 - 1].value == V_EMPTY ||
        tiles[y / 40][x / 40 - 1].value == V_WALL2) {
      x = x - 40;
    } else if (tiles[y / 40][x / 40 - 1].value == V_WALL) {
      asw::sound::play(hitwall, 255, 122, 0);
    } else if (tiles[y / 40][x / 40 - 1].value == V_BOX &&
               tiles[y / 40][x / 40 - 2].value == V_EMPTY) {
      asw::sound::play(boxslide, 255, 122, 0);
      tiles[y / 40][x / 40 - 1].value = V_EMPTY;
      tiles[y / 40][x / 40 - 2].value = V_BOX;
      x = x - 40;
    } else if (tiles[y / 40][x / 40 - 1].value == V_BOX &&
               tiles[y / 40][x / 40 - 2].value != 0) {
      if (tiles[y / 40][x / 40 - 1].value == V_BOX &&
          tiles[y / 40][x / 40 - 2].value == V_ROBOT) {
        asw::sound::play(explode, 255, 122, 0);
        tiles[y / 40][x / 40 - 2].value = V_SCRAP;
        score += 100;
      } else {
        asw::sound::play(boxhitwall, 255, 122, 0);
      }
    } else if (tiles[y / 40][x / 40 - 1].value == V_SCRAP &&
               tiles[y / 40][x / 40 - 2].value == V_EMPTY && haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40][x / 40 - 1].value = V_EMPTY;
      tiles[y / 40][x / 40 - 2].value = V_SCRAP;
      asw::sound::play(sweep, 1000, 122, 0);
    } else if (tiles[y / 40][x / 40 - 1].value == V_SCRAP &&
               tiles[y / 40][x / 40 - 2].value == V_GARBAGECAN &&
               haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40][x / 40 - 1].value = V_EMPTY;
      score += 50;

      if (robotsLeft > 0) {
        robotsLeft -= 1;
      }

      asw::sound::play(sweep, 1000, 122, 0);
      asw::sound::play(trash, 255, 122, 0);
    } else if (tiles[y / 40][x / 40 - 1].value == V_JANITORROOM) {
      if (!haveBroom) {
        tiles[y / 40][x / 40 - 1].value = V_JANITORROOMOPEN;
        asw::sound::play(door, 255, 122, 0);
        haveBroom = true;
      }
    } else if (tiles[y / 40][x / 40 - 1].value == V_JANITORROOMOPEN) {
      if (robotsLeft == 0) {
        doneLevel = 1;
      }
    }
  }

  if (asw::input::wasKeyPressed(asw::input::Key::RIGHT) ||
      asw::input::wasKeyPressed(asw::input::Key::D)) {
    characterRotation = 192;

    if (tiles[y / 40][x / 40 + 1].value == V_EMPTY ||
        tiles[y / 40][x / 40 + 1].value == V_WALL2) {
      x = x + 40;
    } else if (tiles[y / 40][x / 40 + 1].value == V_WALL) {
      asw::sound::play(hitwall, 255, 122, 0);
    } else if (tiles[y / 40][x / 40 + 1].value == V_BOX &&
               tiles[y / 40][x / 40 + 2].value == V_EMPTY) {
      asw::sound::play(boxslide, 255, 122, 0);
      tiles[y / 40][x / 40 + 1].value = V_EMPTY;
      tiles[y / 40][x / 40 + 2].value = V_BOX;
      x = x + 40;
    } else if (tiles[y / 40][x / 40 + 1].value == V_BOX &&
               tiles[y / 40][x / 40 + 2].value != 0) {
      if (tiles[y / 40][x / 40 + 1].value == V_BOX &&
          tiles[y / 40][x / 40 + 2].value == V_ROBOT) {
        asw::sound::play(explode, 255, 122, 0);
        tiles[y / 40][x / 40 + 2].value = V_SCRAP;
        score += 100;
      } else {
        asw::sound::play(boxhitwall, 255, 122, 0);
      }
    } else if (tiles[y / 40][x / 40 + 1].value == V_SCRAP &&
               tiles[y / 40][x / 40 + 2].value == V_EMPTY && haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40][x / 40 + 1].value = V_EMPTY;
      tiles[y / 40][x / 40 + 2].value = V_SCRAP;
      asw::sound::play(sweep, 1000, 122, 0);
    } else if (tiles[y / 40][x / 40 + 1].value == V_SCRAP &&
               tiles[y / 40][x / 40 + 2].value == V_GARBAGECAN &&
               haveBroom == 1 &&
               asw::input::isKeyDown(asw::input::Key::SPACE)) {
      tiles[y / 40][x / 40 + 1].value = V_EMPTY;
      score += 50;

      if (robotsLeft > 0) {
        robotsLeft -= 1;
      }

      asw::sound::play(sweep, 1000, 122, 0);
      asw::sound::play(trash, 255, 122, 0);
    } else if (tiles[y / 40][x / 40 + 1].value == V_JANITORROOM) {
      if (!haveBroom) {
        asw::sound::play(door, 255, 122, 0);
        haveBroom = 1;
        tiles[y / 40][x / 40 + 1].value = V_JANITORROOMOPEN;
      }
    } else if (tiles[y / 40][x / 40 + 1].value == V_JANITORROOMOPEN) {
      if (robotsLeft == 0) {
        doneLevel = 1;
      }
    }
  }

  mousePressed = false;
}

void updateRobots() {
  for (int i = 0; i < 24; i++) {
    for (int t = 0; t < 32; t++) {
      if (tiles[i][t].value == V_ROBOT) {
        int random_integer = asw::random::between(0, 4);

        if (random_integer == 1 && tiles[i - 1][t].value == V_EMPTY) {
          tiles[i - 1][t].value = V_ROBOT;
          tiles[i][t].value = V_EMPTY;
        }

        if (random_integer == 2 && tiles[i][t + 1].value == V_EMPTY) {
          tiles[i][t + 1].value = V_ROBOT;
          tiles[i][t].value = V_EMPTY;
        }

        if (random_integer == 3 && tiles[i + 1][t].value == V_EMPTY) {
          tiles[i + 1][t].value = V_ROBOT;
          tiles[i][t].value = V_EMPTY;
        }

        if (random_integer == 4 && tiles[i][t - 1].value == V_EMPTY) {
          tiles[i][t - 1].value = V_ROBOT;
          tiles[i][t].value = V_EMPTY;
        }

        if (i == y / 40 && t == x / 40) {
          asw::sound::play(oof, 2000, 122, 0);
          lives -= 1;
          x = 40;
          y = 40;
        }
      }
    }
  }
}

// Change tiles
void changeMap() {
  finalFile = "assets/levels/level" + convertInt(currentLevel) + ".map";

  doneLevel = 0;
  haveBroom = 0;
  x = 40;
  y = 40;

  if (currentLevel == 1) {
    robotsLeft = 3;
    levelText = "Welcome to the maze";
  } else if (currentLevel == 2) {
    robotsLeft = 6;
    levelText = "Boxed in";
  } else if (currentLevel == 3) {
    robotsLeft = 6;
    levelText = "Rooms";
  } else if (currentLevel == 4) {
    robotsLeft = 2;
    levelText = "Think, then act";
  } else if (currentLevel == 5) {
    robotsLeft = 1;
    levelText = "A new secret";
  } else if (currentLevel == 6) {
    robotsLeft = 2;
    levelText = "Don't worry";
  } else if (currentLevel == 7) {
    robotsLeft = 4;
    levelText = "The Box";
  } else if (currentLevel == 8) {
    robotsLeft = 0;
    levelText = "Hoarder";
  } else if (currentLevel == 9) {
    robotsLeft = 1;
    levelText = "One Robot";
  } else {
    robotsLeft = 0;
    levelText = "Custom Level";
  }

  if (fexists(finalFile.c_str())) {
    std::ifstream read(finalFile.c_str());

    for (int i = 0; i < 24; i++) {
      for (int t = 0; t < 32; t++) {
        read >> map[i][t];
        tiles[i][t].value = map[i][t];
      }
    }

    resetBlocks(-1, -1);
  } else if (gameScreen == 3) {
    gameScreen = 4;
  }
}

// Updates games
void game() {
  // Splash
  if (gameScreen == 0) {
    return;
  }

  // Menu
  else if (gameScreen == 1) {
    if (helpOpen) {
      if (asw::input::keyboard.anyPressed ||
          asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
        helpOpen = false;
      }
    } else {
      if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
        asw::core::exit = true;
      }
    }

    // Checks for mouse press
    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
      if (start.Hover()) {
        gameScreen = 2;
        setupGame();
        return;
      } else if (help.Hover()) {
        helpOpen = true;
      } else if (quit.Hover()) {
        asw::core::exit = true;
      } else if (mode.Hover()) {
        if (perspective == 0) {
          perspective = 1;
          initGame();
          mode.SetImages("assets/images/buttons/mode_2d.png",
                         "assets/images/buttons/mode_2d_hover.png");
        } else if (perspective == 1) {
          perspective = 0;
          initGame();
          mode.SetImages("assets/images/buttons/mode_3d.png",
                         "assets/images/buttons/mode_3d_hover.png");
        }
      }
    }
  }

  // Level Select
  if (gameScreen == 2) {
    // Change map
    changeMap();

    // Go to menu
    if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
      gameScreen = 1;
      return;
    }

    // Click buttons
    if (collision(asw::input::mouse.x, asw::input::mouse.x, -1, 200,
                  asw::input::mouse.y, asw::input::mouse.y, 0, 960)) {
      if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT) &&
          currentLevel > 1) {
        asw::sound::play(click, 255, 125, 0);
        currentLevel -= 1;
        setupGame();
        rest(140);
      }
    } else if (collision(asw::input::mouse.x, asw::input::mouse.x, 1080, 1280,
                         asw::input::mouse.y, asw::input::mouse.y, 0, 960)) {
      if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
        finalFile =
            "assets/levels/level" + convertInt(currentLevel + 1) + ".map";

        if (fexists(finalFile.c_str())) {
          asw::sound::play(click, 255, 125, 0);
          currentLevel += 1;
          setupGame();
          rest(140);
        }
      }
    } else if (collision(asw::input::mouse.x, asw::input::mouse.x, 320, 978,
                         asw::input::mouse.y, asw::input::mouse.y, 220, 718)) {
      if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
        gameScreen = 3;
        changeMap();
        setupGame();
        return;
      }
    }

    if (asw::input::wasButtonPressed(asw::input::MouseButton::LEFT) &&
        back.Hover() == true) {
      gameScreen = 1;
      return;
    }
  }

  // Ingame Loops
  if (gameScreen == 3) {
    // Go to menu
    if (asw::input::wasKeyPressed(asw::input::Key::ESCAPE)) {
      gameScreen = 1;
      score = 0;
      return;
    }

    // Changes move
    moving = false;

    // Update Clock
    currentTime = clock();

    // Checks if movther
    if (int(currentTime - startTime) / CLOCKS_PER_SEC > (elaspedTime)) {
      moving = true;
    }

    // Updates Elasped Time
    elaspedTime = int(currentTime - startTime) / CLOCKS_PER_SEC;

    // Die
    if (tiles[y / 40][x / 40].value == V_ROBOT) {
      lives -= 1;
      x = 40;
      y = 40;
      asw::sound::play(oof, 255, 122, 0);
    }

    // Updates robots positions
    if (step % 40 == 0) {
      updateRobots();
    }

    // Restart Map
    if (asw::input::wasKeyPressed(asw::input::Key::R)) {
      changeMap();
    }

    // Pause Game
    if (asw::input::wasKeyPressed(asw::input::Key::P)) {
      while (!(asw::input::wasKeyPressed(asw::input::Key::P) ||
               asw::input::wasKeyPressed(asw::input::Key::ESCAPE))) {
        asw::display::clear();
        asw::draw::text(font, "Paused press P to resume",
                        asw::Vec2<float>(420, 440),
                        asw::util::makeColor(0, 0, 0));
        asw::core::update();
        asw::display::present();
      }
    }

    // Finish Level
    if (doneLevel == 1) {
      asw::sound::play(winsound, 255, 122, 0);
      currentLevel += 1;
      lives += 1;
      changeMap();
    }

    // Die
    if (lives <= 0) {
      changeMap();
      score = 0;
      setupGame();
    }

    // Draws Tiles
    for (int i = 0; i < 24; i++) {
      for (int t = 0; t < 32; t++) {
        // Reset blocks
        if (tiles[i][t].value != map[i][t]) {
          resetBlocks(i, t);
        }

        map[i][t] = tiles[i][t].value;
      }
    }

    // Checks for character move
    characterMove();

    // Rests (regulates game speed)
    rest(100);

  } else if (gameScreen == 4) {
    if (asw::input::wasKeyPressed(asw::input::Key::SPACE) ||
        asw::input::wasKeyPressed(asw::input::Key::RETURN) ||
        asw::input::wasKeyPressed(asw::input::Key::ESCAPE) ||
        asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
      gameScreen = 1;
      return;
    }
  }

  // Adds one to step
  step++;
}

// Updates games
void drawGame() {
  // Splash
  if (gameScreen == 0) {
    asw::display::clear();
    asw::draw::sprite(intro, asw::Vec2<float>(0, 0));
    asw::display::present();
    rest(1000);
    asw::display::clear();
    asw::draw::sprite(splash, asw::Vec2<float>(0, 0));
    asw::display::present();
    rest(1000);
    gameScreen = 1;
    asw::display::clear();
    asw::draw::sprite(menu, asw::Vec2<float>(0, 0));
    asw::display::present();
    return;
  }

  // Menu
  else if (gameScreen == 1) {
    // Draws Menu
    asw::draw::sprite(menu, asw::Vec2<float>(0, 0));

    // Draws Buttons
    start.draw();
    help.draw();
    quit.draw();
    mode.draw();

    if (helpOpen) {
      asw::draw::sprite(help_menu, asw::Vec2<float>(0, 0));
    }
  }

  // Level Select
  if (gameScreen == 2) {
    asw::draw::sprite(levelSelect, asw::Vec2<float>(0, 0));
    back.draw();
    asw::draw::stretchSprite(background, asw::Quad<float>(320, 220, 640, 480));
    asw::draw::textCenter(font, "Level: " + std::to_string(currentLevel),
                          asw::Vec2<float>(640, 760),
                          asw::util::makeColor(0, 0, 0));

    asw::draw::textCenter(font, levelText, asw::Vec2<float>(640, 800),
                          asw::util::makeColor(0, 0, 0));

    // Mini tiles tiles
    for (int i = 0; i < 24; i++) {
      for (int t = 0; t < 32; t++) {
        if (tiles[i][t].image[0] != nullptr && tiles[i][t].value != 1) {
          if (perspective == 0) {
            asw::draw::stretchSprite(
                tiles[i][t].image[0],
                asw::Quad<float>(t * 20 + 320, i * 20 + 220, 30, 30));
          } else if (perspective == 1) {
            asw::draw::stretchSprite(
                tiles[i][t].image[0],
                asw::Quad<float>(t * 20 + 320, i * 20 + 220, 20, 20));
          }
        }
      }
    }

    // Click buttons
    if (collision(asw::input::mouse.x, asw::input::mouse.x, -1, 200,
                  asw::input::mouse.y, asw::input::mouse.y, 0, 960)) {
      asw::draw::sprite(levelSelectLeft, asw::Vec2<float>(0, 0));
    } else if (collision(asw::input::mouse.x, asw::input::mouse.x, 1080, 1280,
                         asw::input::mouse.y, asw::input::mouse.y, 0, 960)) {
      asw::draw::sprite(levelSelectRight, asw::Vec2<float>(1080, 0));
    }
  }

  // Ingame Loops
  if (gameScreen == 3) {
    // Draw background
    asw::draw::sprite(background, asw::Vec2<float>(0, 0));

    // Draws Tiles
    for (int i = 0; i < 24; i++) {
      for (int t = 0; t < 32; t++) {
        if (y / 40 == i && x / 40 == t) {
          // Draws Character
          if (perspective == 0) {
            // rotate_sprite( character, x + 5, y + 5,
            // ftofix(characterRotation));
            if (characterRotation == 0) {
              asw::draw::sprite(character[0], asw::Vec2<float>(x, y));
            } else if (characterRotation == 128) {
              asw::draw::sprite(character[1], asw::Vec2<float>(x, y));
            } else if (characterRotation == 64) {
              asw::draw::sprite(character[2], asw::Vec2<float>(x, y));
            } else if (characterRotation == 192) {
              asw::draw::sprite(character[3], asw::Vec2<float>(x, y));
            }
          } else {
            asw::draw::sprite(character[0], asw::Vec2<float>(x, y));
          }

          // Draws broom if needed
          if (asw::input::isKeyDown(asw::input::Key::SPACE) && haveBroom == 1) {
            if (perspective == 0) {
              asw::draw::rotateSprite(broom, asw::Vec2<float>(x + 10, y + 10),
                                      characterRotation);
            } else {
              asw::draw::sprite(broom, asw::Vec2<float>(x, y));
            }
          }
        }

        if (tiles[i][t].image[0] != nullptr) {
          if (perspective == 0) {
            asw::draw::stretchSprite(tiles[i][t].image[0],
                                     asw::Quad<float>(t * 40, i * 40, 60, 60));
          } else {
            asw::draw::stretchSprite(tiles[i][t].image[0],
                                     asw::Quad<float>(t * 40, i * 40, 40, 40));
          }
        }
      }
    }

    // Draws Stats
    asw::draw::text(font, "Score:" + std::to_string(score),
                    asw::Vec2<float>(0, 0),
                    asw::util::makeColor(255, 255, 255));

    asw::draw::textRight(font, "Robots Left:" + std::to_string(robotsLeft),
                         asw::Vec2<float>(1280, 0),
                         asw::util::makeColor(255, 255, 255));

    asw::draw::textCenter(font, "Lives:" + std::to_string(lives),
                          asw::Vec2<float>(640, 0),
                          asw::util::makeColor(255, 255, 255));

  } else if (gameScreen == 4) {
    asw::draw::sprite(winscreen, asw::Vec2<float>(0, 0));
    asw::draw::text(font, std::to_string(score), asw::Vec2<float>(310, 400),
                    asw::util::makeColor(0, 0, 0));

    if (asw::input::wasKeyPressed(asw::input::Key::SPACE) ||
        asw::input::wasKeyPressed(asw::input::Key::RETURN) ||
        asw::input::wasKeyPressed(asw::input::Key::ESCAPE) ||
        asw::input::wasButtonPressed(asw::input::MouseButton::LEFT)) {
      gameScreen = 1;
      return;
    }
  }

  // Adds one to step
  step++;
}

/// Scene stuff to get working in browser
enum class GameState { Main };

class MainScene : public asw::scene::Scene<GameState> {
 public:
  using asw::scene::Scene<GameState>::Scene;

  void update(float deltaTime) override {
    Scene::update(deltaTime);
    game();
  }

  void draw() override { drawGame(); }
};

// Main functions run here
int main() {
  // Initializing
  asw::core::init(1280, 960);
  asw::display::setTitle("Mazes");

  initGame();

  // Starts Game
  auto app = asw::scene::SceneManager<GameState>();
  app.registerScene<MainScene>(GameState::Main, app);
  app.setNextScene(GameState::Main);
  app.start();

  return 0;
}
