#ifndef INIT_H
#define INIT_H

#include "State.h"


class Init : public State {
  public:
    Init();
    virtual ~Init();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
};

#endif // INIT_H
