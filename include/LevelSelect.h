#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include "State.h"
#include "ui/Button.h"

class LevelSelect : public State {
  public:
    LevelSelect();
    virtual ~LevelSelect();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    // Next state
    int next_state = -1;

    // Images
    BITMAP *background;
    BITMAP *cursor[2];

    // Sounds
    SAMPLE *click;

    // Buttons
    Button btnBack, btnLeft, btnRight;

    // Collision
    bool collision (int xMin1, int xMax1, int xMin2, int xMax2, int yMin1, int yMax1, int yMin2, int yMax2);
};

#endif // LEVELSELECT_H
