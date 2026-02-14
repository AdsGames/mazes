#pragma once

#include <asw/asw.h>
#include <fstream>

#include "../button.h"
#include "../globals.h"
#include "./game.h"
#include "./scenes.h"

class LevelSelectScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        back.SetImages("assets/images/buttons/back.png", "assets/images/buttons/back_hover.png");

        background = asw::assets::loadTexture("assets/images/background.png");

        // Sets button positions
        back.SetX(380);
        back.SetY(40);

        // Load font
        font = asw::assets::loadFont("assets/fonts/dosis.ttf", 32);

        // Load sounds
        click = asw::assets::loadSample("assets/sfx/click.wav");

        // Load sprites
        levelSelect = asw::assets::loadTexture("assets/images/levelSelect.png");
        levelSelectLeft = asw::assets::loadTexture("assets/images/levelSelectLeft.png");
        levelSelectRight = asw::assets::loadTexture("assets/images/levelSelectRight.png");

        // Load tilemap
        tilemap.load(std::format("assets/levels/level{}.map", GameScene::level));
    }

    void update(float deltaTime) override
    {
        Scene::update(deltaTime);

        // Go to menu
        if (asw::input::getKeyDown(asw::input::Key::Escape)) {
            sceneManager.setNextScene(GameState::Menu);
            return;
        }

        // Click buttons
        int delta_level = 0;
        if (asw::input::getMouseButtonDown(asw::input::MouseButton::Left)) {
            if (leftQuad.contains(asw::input::mouse.position)) {
                delta_level = -1;
            }

            if (rightQuad.contains(asw::input::mouse.position)) {
                delta_level = 1;
            }

            if (playQuad.contains(asw::input::mouse.position)) {
                sceneManager.setNextScene(GameState::Game);
            }

            if (back.Hover() == true) {
                sceneManager.setNextScene(GameState::Menu);
            }
        }

        // Change level
        if (delta_level != 0) {
            std::string file_path
                = std::format("assets/levels/level{}.map", GameScene::level + delta_level);
            std::ifstream read(file_path.c_str());
            if (!read.fail()) {
                tilemap.load(file_path);
                asw::sound::play(click);
                GameScene::level += delta_level;
            }
        }
    }

    void draw() override
    {
        asw::draw::sprite(levelSelect, asw::Vec2<float>(0, 0));
        back.draw();
        asw::draw::stretchSprite(background, playQuad);

        // Mini tiles tiles
        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                auto& tile = tilemap.at(i, t);
                const auto tile_pos = asw::Vec2<float>((i * 20) + 320, (t * 20) + 220);

                if (!tile.image || tile.value == 1) {
                    continue;
                }

                if (perspective == 0) {
                    auto position = asw::Quad<float>(tile_pos.x, tile_pos.y, 30, 30);
                    asw::draw::stretchSprite(tile.image, position);
                } else if (perspective == 1) {
                    auto position = asw::Quad<float>(tile_pos.x, tile_pos.y, 20, 20);
                    asw::draw::stretchSprite(tile.image, position);
                }
            }
        }

        // Text
        asw::draw::textCenter(font, std::format("Level: {}", GameScene::level),
            asw::Vec2<float>(640, 760), asw::util::makeColor(255, 255, 255));

        asw::draw::textCenter(font, tilemap.level_text, asw::Vec2<float>(640, 800),
            asw::util::makeColor(255, 255, 255));

        // Hovering
        if (leftQuad.contains(asw::input::mouse.position)) {
            asw::draw::sprite(levelSelectLeft, asw::Vec2<float>(0, 0));
        }
        if (rightQuad.contains(asw::input::mouse.position)) {
            asw::draw::sprite(levelSelectRight, asw::Vec2<float>(1080, 0));
        }
    }

private:
    Button back;

    asw::Texture background;
    asw::Font font;
    asw::Sample click;

    asw::Texture levelSelect;
    asw::Texture levelSelectLeft;
    asw::Texture levelSelectRight;

    asw::Quad<float> leftQuad { 0, 0, 200, 960 };
    asw::Quad<float> rightQuad { 1080, 0, 200, 960 };
    asw::Quad<float> playQuad { 320, 220, 640, 480 };
};