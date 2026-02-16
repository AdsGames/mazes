#pragma once

#include <asw/asw.h>
#include <fstream>
#include <memory>

#include "../button.h"
#include "../globals.h"
#include "./game.h"
#include "./scenes.h"

class LevelSelectScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        // BG Image
        auto background = createObject<asw::game::Sprite>();
        background->setTexture(asw::assets::loadTexture("assets/images/background.png"));

        // Sets button positions
        btn_left = createObject<Button>();
        btn_right = createObject<Button>();
        back = createObject<Button>();

        btn_left->setImages("assets/images/arrow_left_hover.png", "assets/images/arrow_left.png");
        btn_right->setImages(
            "assets/images/arrow_right_hover.png", "assets/images/arrow_right.png");
        back->setImages("assets/images/buttons/back.png", "assets/images/buttons/back_hover.png");

        btn_left->transform.position = asw::Vec2<float>(100, 420);
        btn_right->transform.position = asw::Vec2<float>(1280 - 64 - 100, 420);
        back->transform.position = asw::Vec2<float>(380, 40);

        // Load font
        font = asw::assets::loadFont("assets/fonts/dosis.ttf", 32);

        // Load sounds
        click = asw::assets::loadSample("assets/sfx/click.wav");

        // Load sprites
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
            if (btn_left->hover()) {
                delta_level = -1;
            }

            if (btn_right->hover()) {
                delta_level = 1;
            }

            if (playQuad.contains(asw::input::mouse.position)) {
                sceneManager.setNextScene(GameState::Game);
            }

            if (back->hover()) {
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
        Scene::draw();

        asw::draw::rectFill(playQuad, asw::Color(179, 185, 209));

        // Mini tiles tiles
        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                auto& tile = tilemap.at(i, t);
                if (!tile.image || tile.value == 1) {
                    continue;
                }

                const auto tile_pos = asw::Vec2<float>((i * 20) + 320, (t * 20) + 220);
                auto position = asw::Quad<float>(tile_pos.x, tile_pos.y, 30, 30);
                asw::draw::stretchSprite(tile.image, position);
            }
        }

        // Text
        asw::draw::textCenter(font, std::format("Level: {}", GameScene::level),
            asw::Vec2<float>(640, 760), asw::color::white);

        asw::draw::textCenter(
            font, tilemap.level_text, asw::Vec2<float>(640, 800), asw::color::white);
    }

private:
    std::shared_ptr<Button> back;
    std::shared_ptr<Button> btn_left;
    std::shared_ptr<Button> btn_right;

    asw::Font font;
    asw::Sample click;

    asw::Texture levelSelect;

    asw::Quad<float> leftQuad { 0, 0, 200, 960 };
    asw::Quad<float> rightQuad { 1080, 0, 200, 960 };
    asw::Quad<float> playQuad { 320, 220, 640, 480 };
};