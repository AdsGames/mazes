#ifndef TRANSITIONEFFECTS_H
#define TRANSITIONEFFECTS_H

#include <allegro.h>

class TransitionEffects {
  public:
 static void FadeIn (BITMAP * bmp_orig, int speed);
    static void FadeOut (int speed);

};

#endif // TRANSITIONEFFECTS_H
