#ifndef ENDSCREEN_H
#define ENDSCREEN_H

#include "State.h"


class EndScreen : public State {
  public:
    EndScreen();
    virtual ~EndScreen();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    BITMAP *winscreen;
};

#endif // ENDSCREEN_H
