#ifndef MENU_H
#define MENU_H

#include "State.h"


class Menu : public State {
  public:
    Menu();
    virtual ~Menu();

    virtual void draw(BITMAP* buffer) override;
    virtual void update(StateEngine &engine) override;

  private:
};

#endif // MENU_H
