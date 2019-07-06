#ifndef BUTTON_H
#define BUTTON_H

#include <allegro.h>
#include <string>
#include <functional>

class Button {
  public:
    Button();
    Button (int x, int y);
    ~Button();

    void Update();

    void SetImages (std::string image1, std::string image2);

    int GetX() const;
    int GetY() const;

    void SetOnClick (std::function<void()> func);

    void Draw (BITMAP *buffer);

    bool Hover() const;

  private:
    std::function<void (void)> OnClick;

    int x, y;
    int width, height;

    BITMAP *images[2];
};

#endif

