#pragma once

#include <asw/asw.h>
#include <fstream>
#include <memory>

#include "../button.h"
#include "../colors.h"
#include "../globals.h"
#include "../tilemap.h"
#include "./game.h"
#include "./scenes.h"

class LevelSelectScene : public asw::scene::Scene<GameState> {
public:
    using asw::scene::Scene<GameState>::Scene;

    void init() override
    {
        // Load font
        font = asw::assets::loadFont("assets/fonts/jersey-10.ttf", 48);

        // BG Image
        auto background = createObject<asw::game::Sprite>();
        background->setTexture(asw::assets::loadTexture("assets/images/background.png"));

        // Sets button positions
        btn_left = createObject<Button>();
        btn_left->setImages(asw::assets::loadTexture("assets/images/arrow_left_hover.png"),
            asw::assets::loadTexture("assets/images/arrow_left.png"));
        btn_left->transform.position = asw::Vec2<float>(100, 420);

        btn_right = createObject<Button>();
        btn_right->setImages(asw::assets::loadTexture("assets/images/arrow_right_hover.png"),
            asw::assets::loadTexture("assets/images/arrow_right.png"));
        btn_right->transform.position = asw::Vec2<float>(1280 - 64 - 100, 420);

        back = createObject<Button>();
        back->setColor(palette::red);
        back->setText("Back");
        back->setFont(font);
        back->transform.setPosition(40, 856);
        back->transform.setSize(200, 64);

        // Level text
        level_text = createObject<asw::game::Text>();
        level_text->setFont(font);
        level_text->setText("");
        level_text->setColor(palette::white);
        level_text->setJustify(asw::TextJustify::CENTER);
        level_text->transform.setPosition(640, 760);

        // Load sounds
        click = asw::assets::loadSample("assets/sfx/click.wav");

        // Load sprites
        // Load tilemap
        tilemap.load(std::format("assets/levels/level{}.json", GameScene::level));
        tilemap.setRenderConfig(
            { .tile_size = 20, .render_size = 30, .offset_x = 320, .offset_y = 220 });
    }

    void update(float dt) override
    {
        Scene::update(dt);

        // Level text
        level_text->setText(std::format("Level {}: {}", GameScene::level, tilemap.getLevelText()));

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
            const auto file_path
                = std::format("assets/levels/level{}.json", GameScene::level + delta_level);
            if (tilemap.load(file_path)) {
                asw::sound::play(click);
                GameScene::level += delta_level;
            }
        }
    }

    void draw() override
    {
        Scene::draw();

        tilemap.renderBackground();

        // Mini tiles tiles
        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                tilemap.render({ i, t });
            }
        }
    }

private:
    std::shared_ptr<Button> back;
    std::shared_ptr<Button> btn_left;
    std::shared_ptr<Button> btn_right;

    asw::Font font;
    asw::Sample click;

    asw::Texture level_select;
    std::shared_ptr<asw::game::Text> level_text;
    asw::Quad<float> playQuad { 320, 220, 640, 480 };

    TileMap tilemap;
};