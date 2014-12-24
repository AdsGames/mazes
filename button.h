#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>
#include <string>
#include <time.h>

using namespace std;

class Button{

public:
       Button();
       ~Button();

       void SetImages( char image1[], char image2[]);

       void SetHover(bool newHover);
       bool GetHover();

       void draw(BITMAP* tempBitmap);
       void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw);

       void SetX( int newValue);
       void SetY( int newValue);

       void SetType(int newType);
       void SetValue(int newValue);

       bool CheckHover();

       int GetX();
       int GetY();

private:
       int button_height;
       int button_width;

       int x;
       int y;

       bool hover;

       BITMAP* images[2][2];
};

#endif

