#ifndef GAME_H
#define GAME_H

#include "State.h"

class Game : public State {
  public:
    Game();
    virtual ~Game();

    virtual void draw(BITMAP* buffer) override;
    virtual void update(StateEngine &engine) override;
  private:
};

#endif // GAME_H
