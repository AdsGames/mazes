#include "block.h"

using namespace std;

Block::Block(){
    
}

Block::~Block(){
    delete [] images[0][0], images[0][1], sound;
}

void Block::SetImages( char image1[], char image2[]){  
   images[0][0]= load_bitmap(image1, NULL);
   images[0][1]= load_bitmap(image2, NULL); 
}

int Block::GetType(){
    return type;
}
       
int Block::GetX(){
    return x;
}
       
int Block::GetY(){
    return y;
}

bool Block::GetSelected(){
    return selected;
}

void Block::SetType(int newType){
    type = newType;
}

void Block::SetSelected(bool newSelected){
    selected = newSelected;
}

void Block::SetX( int newValue){
    x = newValue;
}

void Block::SetY( int newValue){
    y = newValue;
}

void Block::draw(BITMAP* tempBitmap){  
          if(frame==0||frame==1||frame==2||frame==3||frame==4){
               frame+=1;
               DrawNewSprite( tempBitmap, images[0][0]);
          }
          else if(frame==5||frame==6||frame==7||frame==8||frame==9||frame==10||frame==11||frame==12||frame==13||frame==14||frame==15){
               frame+=1;
               DrawNewSprite( tempBitmap, images[0][1]);
          if(frame==16){frame=0;}
          }                            
}

void Block::Change(bool flash){
     if(flash==false){
          if(GetType()==0){SetImages("images/blocks/red.bmp","images/blocks/red.bmp");}
          else if(GetType()==1){SetImages("images/blocks/orange.bmp","images/blocks/orange.bmp");}
          else if(GetType()==2){SetImages("images/blocks/yellow.bmp","images/blocks/yellow.bmp");}
          else if(GetType()==3){SetImages("images/blocks/green.bmp","images/blocks/green.bmp");}
          else if(GetType()==4){SetImages("images/blocks/blue.bmp","images/blocks/blue.bmp");}
          else if(GetType()==5){SetImages("images/blocks/purple.bmp","images/blocks/purple.bmp");}
          else if(GetType()==6){SetImages("images/blocks/none.bmp","images/blocks/none.bmp");}
     }
     else{
          if(GetType()==0){SetImages("images/blocks/red.bmp","images/blocks/flash.bmp");}
          else if(GetType()==1){SetImages("images/blocks/orange.bmp","images/blocks/flash.bmp");}
          else if(GetType()==2){SetImages("images/blocks/yellow.bmp","images/blocks/flash.bmp");}
          else if(GetType()==3){SetImages("images/blocks/green.bmp","images/blocks/flash.bmp");}
          else if(GetType()==4){SetImages("images/blocks/blue.bmp","images/blocks/flash.bmp");}
          else if(GetType()==5){SetImages("images/blocks/purple.bmp","images/blocks/flash.bmp");}
          else if(GetType()==6){SetImages("images/blocks/none.bmp","images/blocks/none.bmp");}
     }     
}

void Block::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw){
          draw_sprite(tempBitmap, spriteToDraw, GetX(), GetY());  
}
