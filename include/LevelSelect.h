#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include "State.h"
#include "button.h"

class LevelSelect : public State {
  public:
    LevelSelect();
    virtual ~LevelSelect();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    // Images
    BITMAP *levelSelect;
    BITMAP *levelSelectLeft;
    BITMAP *levelSelectRight;
    BITMAP *background;
    BITMAP *cursor[2];

    // Sounds
    SAMPLE *click;

    // Buttons
    Button back;

    // Collision
    bool collision (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
};

#endif // LEVELSELECT_H
