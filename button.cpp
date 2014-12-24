#include "button.h"

using namespace std;

Button::Button(){
    button_height=120;
    button_width=520;
}

Button::~Button(){

}

void Button::SetImages( char image1[], char image2[]){
   images[0][0]= load_bitmap(image1, NULL);
   images[0][1]= load_bitmap(image2, NULL);
}

void Button::SetHover(bool newHover){
    hover = newHover;
}

bool Button::GetHover(){
    return hover;
}

bool Button::CheckHover(){
     if(mouse_x>GetX() && mouse_x<GetX()+button_width && mouse_y>GetY() && mouse_y<GetY()+button_height){
          return true;
     }
     else{
          return false;
     }
}

void Button::SetX( int newValue){
    x = newValue;
}

void Button::SetY( int newValue){
    y = newValue;
}

int Button::GetX(){
    return x;
}

int Button::GetY(){
    return y;
}

void Button::draw(BITMAP* tempBitmap){
  if(CheckHover()==true){
       DrawNewSprite( tempBitmap, images[0][1]);
  }
  else{
       DrawNewSprite( tempBitmap, images[0][0]);
  }
}

void Button::DrawNewSprite( BITMAP* tempBitmap, BITMAP* spriteToDraw){
  draw_sprite(tempBitmap, spriteToDraw, GetX(), GetY());
}
