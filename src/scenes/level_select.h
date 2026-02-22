#pragma once

#include <asw/asw.h>
#include <fstream>
#include <memory>

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
        const auto font = asw::assets::load_font("assets/fonts/jersey-10.ttf", 48);

        // Load sounds
        const auto click = asw::assets::load_sample("assets/sfx/click.wav");

        // Build UI tree
        ui_root_ = asw::ui::Root();
        ui_root_.root.transform.set_size(1280, 960);
        ui_root_.root.bg_image = asw::assets::load_texture("assets/images/background.png");

        // Setup buttons
        auto& btn_left = ui_root_.root.add_child<asw::ui::Button>();
        btn_left.text = "<";
        btn_left.font = font;
        btn_left.transform.set_position(100, 420);
        btn_left.transform.set_size(64, 64);
        btn_left.on_click = [this, click]() {
            const auto file_path = std::format("assets/levels/level{}.json", GameScene::level - 1);
            if (tilemap_.load(file_path)) {
                asw::sound::play(click);
                GameScene::level--;
            }
        };

        auto& btn_play = ui_root_.root.add_child<asw::ui::Button>();
        btn_play.transform.set_position(320, 220);
        btn_play.transform.set_size(640, 480);
        btn_play.on_click = [this]() { manager.set_next_scene(GameState::Game); };

        auto& btn_right = ui_root_.root.add_child<asw::ui::Button>();
        btn_right.text = ">";
        btn_right.font = font;
        btn_right.transform.set_position(1280 - 64 - 100, 420);
        btn_right.transform.set_size(64, 64);
        btn_right.on_click = [this, click]() {
            const auto file_path = std::format("assets/levels/level{}.json", GameScene::level + 1);
            if (tilemap_.load(file_path)) {
                asw::sound::play(click);
                GameScene::level++;
            }
        };

        auto& back = ui_root_.root.add_child<asw::ui::Button>();
        back.text = "Back";
        back.font = font;
        back.transform.set_position(40, 856);
        back.transform.set_size(200, 64);
        back.on_click = [this]() { manager.set_next_scene(GameState::Menu); };

        // Level text
        auto& level_text = ui_root_.root.add_child<asw::ui::Label>();
        level_text.font = font;
        level_text.color = palette::white;
        level_text.justify = asw::TextJustify::Center;
        level_text.transform.set_position(640, 760);
        level_text_ref_ = &level_text;

        // Load sprites
        // Load tilemap
        tilemap_.load(std::format("assets/levels/level{}.json", GameScene::level));
        tilemap_.setRenderConfig(
            { .tile_size = 20, .render_size = 30, .offset_x = 320, .offset_y = 220 });
    }

    void update(float dt) override
    {
        Scene::update(dt);

        // Text
        level_text_ref_->text
            = std::format("Level {}: {}", GameScene::level, tilemap_.getLevelText());

        // UI State
        ui_root_.update();

        // Go to menu
        if (asw::input::get_key_down(asw::input::Key::Escape)) {
            manager.set_next_scene(GameState::Menu);
        }
    }

    void draw() override
    {
        Scene::draw();

        ui_root_.draw();

        tilemap_.renderBackground();

        // Mini tiles tiles
        for (int i = 0; i < TileMap::WIDTH; i++) {
            for (int t = 0; t < TileMap::HEIGHT; t++) {
                tilemap_.render({ i, t });
            }
        }
    }

private:
    asw::ui::Root ui_root_;
    asw::ui::Label* level_text_ref_;

    TileMap tilemap_;
};