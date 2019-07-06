#ifndef MENU_H
#define MENU_H

#include "State.h"
#include "ui/Button.h"

class Menu : public State {
  public:
    Menu();
    virtual ~Menu();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    // Menu
    int next_state;
    bool show_help;

    // Buttons
    Button start;
    Button help;
    Button quit;

    // Images
    BITMAP *menu;
    BITMAP *help_menu;
    BITMAP *cursor[2];
};

#endif // MENU_H
