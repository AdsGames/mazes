#include "Init.h"

Init::Init() {
  set_window_title ("Setting up");

  set_display_switch_mode (SWITCH_BACKGROUND);

  // Setup number generator
  srand (time (nullptr));

  set_window_title ("Mazes");
}

void Init::update (StateEngine &engine) {
  setNextState (engine, StateEngine::STATE_INTRO);
}

void Init::draw (BITMAP *buffer) {
  clear_to_color (buffer, 0x000000);
}

Init::~Init() {

}
