#include "AssetLoader.h"

#include <logg.h>
#include <loadpng.h>

// Error reporting
void AssetLoader::Abort (std::string message) {
  if (screen != NULL) {
    set_gfx_mode (GFX_TEXT, 0, 0, 0, 0);
  }

  allegro_message ("%s.\n %s\n", message.c_str(), allegro_error);
  exit (-1);
}

// Load image
BITMAP* AssetLoader::LoadPng (std::string path) {
  BITMAP *temp = nullptr;

  if (! (temp = load_png (path.c_str(), nullptr)))
    Abort ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load ogg
SAMPLE* AssetLoader::LoadOgg (std::string path) {
  SAMPLE *temp = nullptr;

  if (! (temp = logg_load (path.c_str())))
    Abort ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load sample
SAMPLE* AssetLoader::LoadWav (std::string path) {
  SAMPLE *temp = nullptr;

  if (! (temp = load_sample (path.c_str())))
    Abort ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}

// Load font
FONT* AssetLoader::LoadPcxFont (std::string path) {
  FONT *temp = nullptr;

  if (! (temp = load_font (path.c_str(), nullptr, nullptr)))
    Abort ("Cannot find image (" + path + ") \n Please check your files and try again");

  return temp;
}
