#ifndef INTRO_H
#define INTRO_H

#include "State.h"

#include "utility/Timer.h"

class Intro : public State {
  public:
    Intro();
    virtual ~Intro();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    BITMAP *intro;
    BITMAP *splash;
    Timer tmr;
};

#endif // INTRO_H
