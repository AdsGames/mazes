#ifndef TRANSITIONEFFECTS_H
#define TRANSITIONEFFECTS_H

#include <allegro.h>

class TransitionEffects {
  public:
    static void highcolor_fade_in (BITMAP *bmp_orig, int speed);
    static void highcolor_fade_out (int speed);

};

#endif // TRANSITIONEFFECTS_H
