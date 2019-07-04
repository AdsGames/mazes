#ifndef MENU_H
#define MENU_H

#include "State.h"
#include "button.h"

class Menu : public State {
  public:
    Menu();
    virtual ~Menu();

    virtual void draw (BITMAP *buffer) override;
    virtual void update (StateEngine &engine) override;

  private:
    // Buttons
    Button start;
    Button help;
    Button quit;

    // Images
    BITMAP *menu;
    BITMAP *help_menu;
    BITMAP *cursor[2];

    // Menu
    int selectorY, selectorX, newSelectorY, selected_object;
    int cursor_x, cursor_y;
    int menu_view_x, menu_view_y;
};

#endif // MENU_H
