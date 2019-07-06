#ifndef TOOLS_H
#define TOOLS_H

#include <allegro.h>
#include <loadpng.h>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

// Key or joy button pressed
extern bool key_down();
extern bool button_down();

//Collision
extern bool collision (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);

//Random number generator
extern int random (int low, int high);

#endif // TOOLS_H
