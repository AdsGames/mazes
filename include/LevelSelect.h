#ifndef LEVELSELECT_H
#define LEVELSELECT_H

#include "State.h"


class LevelSelect : public State {
  public:
    LevelSelect();
    virtual ~LevelSelect();

    virtual void draw(BITMAP* buffer) override;
    virtual void update(StateEngine &engine) override;

  private:
};

#endif // LEVELSELECT_H
