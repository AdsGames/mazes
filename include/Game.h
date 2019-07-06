#ifndef GAME_H
#define GAME_H

#include "State.h"

#include <logg.h>

#include <regex>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"

#include "id.h"

class Game : public State {
  public:
    Game();
    virtual ~Game();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

    void charactermove();
  private:
    //Define bitmaps
    BITMAP *character[4];
    BITMAP *background;
    BITMAP *robot;
    BITMAP *wall;
    BITMAP *fakeWall;
    BITMAP *ground;
    BITMAP *box;

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

    //Main Map
    int map[24][32] = {0};

    //Tiles on screen
    struct tile {
      BITMAP *image[2];
      int value;
      int type;
      int dir;
    };

    tile tiles[24][32];

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

    //Timers
    clock_t startTime;
    clock_t currentTime;

    void resetBlocks (int newI, int newT);
    bool changeMap();
};

#endif // GAME_H
