#ifndef ASSETLOADER_H
#define ASSETLOADER_H

#include <allegro.h>
#include <string>

class AssetLoader {
  public:
    // Load image
    static BITMAP *LoadPng (std::string path);

    // Load ogg
    static SAMPLE *LoadOgg (std::string path);

    // Load sample
    static SAMPLE *LoadWav (std::string path);

    // Load font
    static FONT *LoadPcxFont (std::string path);

  private:
    // Error reporting
    static void Abort (std::string message);
};

#endif // ASSETLOADER_H
