#ifndef BLOCK_H
#define BLOCK_H

#include <allegro.h>
#include <string>
#include <time.h>

using namespace std;

class Block{

public:
       Block();
       ~Block();
       
       void SetImages( char image1[], char image2[]);

       int GetY();     
       int GetX();
       
       void SetX( int newValue);
       void SetY( int newValue);
       
       int GetType();
       void SetType(int newType); 
       
       bool GetSelected();
       void SetSelected(bool newSelected);
       
       void draw(BITMAP* tempBitmap);
       void DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw);  
       
       void Change(bool flash);
              
private:
        
       int x;
       int y;
       
       int type;
       
       int frame;
       
       bool selected;
       
       BITMAP* images[2][2]; 
       SAMPLE* sound;
};

#endif

