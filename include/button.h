#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>

class Button {
  public:
    Button();
    ~Button();

    void SetImages (const char *image1, const char *image2);

    void draw (BITMAP *tempBitmap);

    void SetX (int newValue);
    void SetY (int newValue);

    void SetType (int newType);
    void SetValue (int newValue);

    bool Hover() const;

    int GetX() const;
    int GetY() const;

  private:
    int x, y;
    int width, height;

    BITMAP *images[2];
};

#endif

